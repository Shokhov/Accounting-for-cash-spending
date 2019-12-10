/*Accounting-for-cash-spending*/
#include <stdio.h>
#include <stdlib.h>

//Функции для работы с базой данных
#include "BD_ACS.h"   // можно не добавлять gcc нормально обработает и найдет нужные функции, но не во всех компиляторах работает
#include "BD_ACS.c"   // обязательно к добовлению

/*Прототипы*/
/*Функции Меню*/
void menu(); /*меню*/

/*Функции манипуляции с данными*/
void write_data(); /*Ввод данных*/
void change_data();/*Изменить данные*/
void remove_data();/*Удалить запись*/

/*Функции вывода*/
int display_all_records();/*Вывод всех записей*/
void information_output();/*вывод информации*/

/*Функции для доп. манипуляции с данными*/
int code_selection(int *num);/*Выбор кода записи*/
int change_all_entries(int index,int max_code, double temp_info_amount, int is_remove);/*Изменяем все последующие среднии значения записей, так как изменили сумму где-то в другом месте*/
void date_entry(short index, short quantity, char *c, short num, char symbol);/*разбиваем число на символы и записываем в дату*/



int main()
{
    if(BD_check()==3){return Error;} // Проверяем базу данных. Ошибка выходим
    if(BD_out(BD_file,&Info_temp,0,size_info,-1)==Error_no_file){return Error;} // Получаем начальную информацию

    menu(); //Запуск меню

    printf("\nВыход\n");
    return OK;
}

/*-----Функции Меню--------------------------------------------------------------------*/
void menu()
{
    int choice;
    information_output(); /*выводим информацию*/

    while(1){
        printf("\nМеню:\n"
               "    1) Вывести информацию\n"
               "    2) Записать расходы\n"
               "    3) Изменить запись\n"
               "    4) Удалить запись\n"
               "    5) Вывести все записи\n"
               "Выберите пункт меню: ");

        if(scanf("%d",&choice) != 1){break;} //вводим число иначе выходим

        switch (choice) {
            case 1: information_output(); break;
            case 2: write_data(); break;
            case 3: change_data(); break;
            case 4: remove_data(); break;
            case 5: display_all_records(); break;
            default: /*Выход*/ return;
        }
    }


}


/*-----Функции манипуляции с данными--------------------------------------------------------------------*/
/*Ввод данных*/
void write_data()
{
    int date;

    printf("\nЗапись данных:\n");
    /*Записываем дату*/
    while(1){ printf("Введите день месяца: "); if(scanf("%d",&date)!=1 || date<1 || date>31){continue;} break; } //ввод дня месяца
    date_entry(0,2,(char *)Table_temp.date,date,'.');
    while(1){ printf("Введите номер месяца: "); if(scanf("%d",&date)!=1 || date<1 || date>12){continue;} break; } //ввод номера месяца
    date_entry(3,2,(char *)Table_temp.date,date,'.');
    while(1){ printf("Введите год: "); if(scanf("%d",&date)!=1 || date<1975 || date>9999){continue;} break; } //ввод года
    date_entry(6,4,(char *)Table_temp.date,date,'\0');

    while(1){
        while(1){ printf("Введите сумму: "); if(scanf("%lf",&Table_temp.amount)!=1){continue;} break; } //ввод суммы
        if(!Table_temp.amount){break;}

        Table_temp.code = Info_temp.new_code++;
        Table_temp.number = ++Info_temp.number_of_records;
        Table_temp.average_amount = (Info_temp.average_amount = (Info_temp.amount += Table_temp.amount)/Info_temp.number_of_records);
        Table_temp.limit_exceeded = 48+(Table_temp.amount>Info_temp.limit);

        BD_add(BD_file,&Table_temp,size_info,size_table);
        BD_in_info(Info_temp.new_code,Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
    }
}

/*Изменить данные*/
void change_data()
{
    int index,choice;
    double temp_amount,def_amount,temp_info_amount;

    if(display_all_records()){return;}
    if(code_selection(&index)){return;}

    printf("\nПоля для изменения:\n    1) Дата\n    2) Сумма\nВыберите поле: ");
    if(scanf("%d",&choice) != 1 || choice<1 || choice>2){return;}

    switch(choice){
        case 1:
            if(BD_out(BD_file,&Table_temp,size_info,size_table,index)==Error_no_file){break;} //Получаем данную запись, если она есть

            /*Записываем дату*/
            int date;
            while(1){ printf("Введите день месяца: "); if(scanf("%d",&date)!=1 || date<1 || date>31){continue;} break; } //ввод дня месяца
            date_entry(0,2,(char *)Table_temp.date,date,'.');
            while(1){ printf("Введите номер месяца: "); if(scanf("%d",&date)!=1 || date<1 || date>12){continue;} break; } //ввод номера месяца
            date_entry(3,2,(char *)Table_temp.date,date,'.');
            while(1){ printf("Введите год: "); if(scanf("%d",&date)!=1 || date<1975 || date>9999){continue;} break; } //ввод года
            date_entry(6,4,(char *)Table_temp.date,date,'\0');

            BD_change(BD_file,&Table_temp,size_info,size_table,index); //изменяем данные
        break;
        case 2:
            printf("Введите сумму: ");
            if(scanf("%lf",&temp_amount)!=1 || !temp_amount){return;}

            //Получаем данную запись, если она есть
            if(BD_out(BD_file,&Table_temp,size_info,size_table,index)!=Error_no_file){

                def_amount = temp_amount - Table_temp.amount; // разница сумм
                if(!def_amount){return;}

                //находим общую сумму на тот момент [Кол-во_записей*Сред.]
                temp_info_amount = Table_temp.number*Table_temp.average_amount+def_amount;

                //Изменяем данные
                Table_temp.average_amount = temp_info_amount/Table_temp.number;
                Table_temp.amount = temp_amount;
                Table_temp.limit_exceeded = 48+(temp_amount>Info_temp.limit);
                BD_change(BD_file,&Table_temp,size_info,size_table,index);

                /*Изменяем все последующие среднии значения записей, так как изменили сумму где-то в другом месте*/
                change_all_entries(index,Info_temp.new_code,temp_info_amount,0);

                Info_temp.amount += def_amount;
                Info_temp.average_amount = Info_temp.amount/Info_temp.number_of_records;
                BD_in_info(Info_temp.new_code,Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
            }
        break;
    }
}

/*Удалить запись*/
void remove_data()
{
    int index;
    if(display_all_records()){return;}
    if(code_selection(&index)){return;}

    if(BD_out(BD_file,&Table_temp,size_info,size_table,index)==Error_no_file){return;} // читаем запись
    Info_temp.amount -= Table_temp.amount;

    change_all_entries(index,Info_temp.new_code,Table_temp.average_amount*Table_temp.number-Table_temp.amount,-1); // все среднии значения меняем

    BD_remove(BD_file,size_info,size_table,index); // удаляем запись

    //последствия удаления
    Info_temp.number_of_records--;
    Info_temp.average_amount = Info_temp.amount/Info_temp.number_of_records;
    BD_in_info(Info_temp.new_code,Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
}

/*-----Функции вывода--------------------------------------------------------------------*/
/*Вывод всех записей*/
int display_all_records()
{
    int rez_out=0;
    printf("\n|Номер| Код |   Дата   |  Сумма  | Сред. |Лимит|\n");
    while(BD_output(BD_file,&Table_temp,size_info,size_table)){
        if(Table_temp.code == -1){continue;}
        printf("|%5ld|%5ld|%s|%7.2lf|%5.2lf|%5c|\n",Table_temp.number,Table_temp.code,Table_temp.date,Table_temp.amount,Table_temp.average_amount,Table_temp.limit_exceeded);
        rez_out++;
    }
    if(!rez_out){printf("Нет записей\n");return Error_no_file;}

    return OK;
}

/*вывод информации*/
void information_output()
{
    printf("\nИнформация:\n"
           "    number of records: %ld\n"
           "    amount: %.2lf\n"
           "    average amount: %.2lf\n"
           "    limit: %d\n",Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
}

/*-----Функции для доп. манипуляции с данными--------------------------------------------------------------------*/
/*Выбор кода записи*/
int code_selection(int *num)
{
    printf("\nВыберите код записи: ");
    if(scanf("%d",num) != 1 || *num<0 || *num>=Info_temp.new_code){return Error_no_file;}
    return OK;
}

/*Изменяем все последующие среднии значения записей, так как изменили сумму где-то в другом месте*/
int change_all_entries(int index, int max_code, double temp_info_amount, int is_remove)
{
    for(long i=index+1;i<max_code;i++){
        if(BD_out(BD_file,&Table_temp,size_info,size_table,i)==Error_no_file){continue;} //Получаем запись, если есть

        temp_info_amount += Table_temp.amount;
        Table_temp.number += is_remove;
        Table_temp.average_amount = temp_info_amount/Table_temp.number;


        BD_change(BD_file,&Table_temp,size_info,size_table,i); //изменяем запись
    }
    return OK;
}

/*разбиваем число на символы и записываем в дату*/
void date_entry(short index, short quantity, char *c, short num, char symbol)
{
    for(int i=quantity;i>0;i--){
        *(c+index+i-1) = 48+(num%10);
        num/=10;
    }
    *(c+index+quantity) = symbol;
}


