/*Accounting-for-cash-spending*/
#include <stdio.h>
#include <stdlib.h>

//Функции для работы с базой данных
#include "BD_ACS.h"
#include "BD_ACS.c"

/*Прототипы*/
void display_all_records();

void write_data();
void date_entry(short index, short quantity, char *c, short num, char symbol);

int main()
{
    if(BD_check()==3){return Error;}

    if(BD_out(BD_file,&Info_temp,0,size_info,-1)==2){return Error;}
    printf("number of records: %ld\namount: %.2lf\naverage amount: %.2lf\nlimit: %d\n",Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);

    display_all_records();

    for(int i=0;i<3;i++){
        write_data();
    }

    display_all_records();


    printf("\nnumber of records: %ld\namount: %.2lf\naverage amount: %.2lf\nlimit: %d\n",Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);






    return OK;
}

/*Вывод всех записей*/
void display_all_records()
{
    int rez_out=0;
    printf("\n|Код|   Дата   |  Сумма  | Сред. |Лимит|\n");
    while(BD_output(BD_file,&Table_temp,size_info,size_table)){
        printf("|%3ld|%s|%7.2lf|%5.2lf|%5c|\n",Table_temp.code,Table_temp.date,Table_temp.amount,Table_temp.average_amount,Table_temp.limit_exceeded);
        rez_out++;
    }
    if(!rez_out){printf("Нет записей\n");}
}

/*Ввод данных*/
void write_data()
{
    int in_num;

    printf("\nЗапись данных:\n");
    /*Записываем дату*/
    while(1){ printf("Введите день месяца: "); if(scanf("%d",&in_num)!=1 || in_num<1 || in_num>31){continue;} break; } //ввод дня месяца
    date_entry(0,2,(char *)Table_temp.date,in_num,'.');
    while(1){ printf("Введите номер месяца: "); if(scanf("%d",&in_num)!=1 || in_num<1 || in_num>12){continue;} break; } //ввод номера месяца
    date_entry(3,2,(char *)Table_temp.date,in_num,'.');
    while(1){ printf("Введите год: "); if(scanf("%d",&in_num)!=1 || in_num<1975 || in_num>9999){continue;} break; } //ввод года
    date_entry(6,4,(char *)Table_temp.date,in_num,'\0');


    while(1){
        while(1){ printf("Введите сумму: "); if(scanf("%lf",&Table_temp.amount)!=1){continue;} break; } //ввод суммы
        if(!Table_temp.amount){break;}

        Table_temp.code = ++Info_temp.number_of_records;
        Table_temp.average_amount = (Info_temp.average_amount = (Info_temp.amount += Table_temp.amount)/Info_temp.number_of_records);
        Table_temp.limit_exceeded = 48+(Table_temp.amount>Info_temp.limit);

        BD_add(BD_file,&Table_temp,size_info,size_table);
        BD_in_info(Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
    }


}
/*разбиваем число на символы и записываем в дату*/
void date_entry(short index, short quantity, char *c, short num, char symbol){
    for(int i=quantity;i>0;i--){
        *(c+index+i-1) = 48+(num%10);
        num/=10;
    }
    *(c+index+quantity) = symbol;
}








