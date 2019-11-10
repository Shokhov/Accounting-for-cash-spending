#include <stdio.h>
#include <stdlib.h>

/*Функии для работы с БД*/
#include "BD_MaSoS.h"
#include "BD_MaSoS.c"
/*Структура БД*/
#include "Configurator.h"
#include "Configurator.c"

void Menu();

void add_record_to_table();
void Add_Employees();
void Add_Nomenclature();
void Add_Stock();
void Add_Counterparties();
void Add_Management();
void Add_Deliveries();
void Add_Leftovers();

void change_record_to_tabel();
void Change_Employees(int rez_out);
void Change_Nomenclature(int rez_out);
void Change_Deliveries(int rez_out);

void remove_record_to_table();

void out_record_to_table();

int test_sravnenie(int kod0, int kod1, char *name0, char *name1, int age0, int age1){
    if(kod0 != kod1) return 1;
    if(age0 != age1) return 2;
    for(int i=0;i<20;i++){
        if(name0[i] != name1[i]) return 3;
        if(name0[i] == '\0') return 0;
    }
    return 4;
}
void test(){
    /*начальные данные*/
    if(fopen("test.bin","wb")){printf("\nTEST:\n");}
    char *filename = "test.bin";
    int rez_in, rez_out;
    struct A{ int kod; char name[20]; unsigned short age; };

    struct A a0 = {1,"a0",10};
    struct A a1;
    struct A b0 = {3,"b0",30};
    struct A b1;
    struct A c0 = {6,"c0",60};
    struct A c1;

    /*Проверка файлов БД*/
    /*Проверка BD_add*/
    printf("\nBD_add: ");
    rez_in = BD_add(filename,&a0,sizeof(a0));
    rez_out =BD_out(filename,&a1,sizeof(a1),0);
    if(!test_sravnenie(a0.kod,a1.kod,a0.name,a1.name,a0.age,a1.age)) printf("[OK] ");
    else printf("[ERR] ");
    rez_in = BD_add(filename,&b0,sizeof(b0));
    rez_out =BD_out(filename,&b1,sizeof(b1),1);
    if(!test_sravnenie(b0.kod,b1.kod,b0.name,b1.name,b0.age,b1.age)) printf("[OK] ");
    else printf("[ERR] ");
    rez_in = BD_add(filename,&c0,sizeof(c0));
    rez_out =BD_out(filename,&c1,sizeof(c1),2);
    if(!test_sravnenie(c0.kod,c1.kod,c0.name,c1.name,c0.age,c1.age)) printf("[OK] ");
    else printf("[ERR] ");

    /*Проверка BD_output*/
    printf("\nBD_output(3):\n");
    while(rez_out=BD_output(filename,&a0,sizeof(a0))){
        printf("%i. kod: %i | name: %s | age: %i\n",rez_out,a0.kod,a0.name,a0.age);
    }
    printf("-----------------");
    printf("\nBD_output(6):\n");
    rez_in = BD_add(filename,&a1,sizeof(a1));
    rez_in = BD_add(filename,&b1,sizeof(b1));
    rez_in = BD_add(filename,&c1,sizeof(c1));
    while(rez_out=BD_output(filename,&a0,sizeof(a0))){
        printf("%i. kod: %i | name: %s | age: %i\n",rez_out,a0.kod,a0.name,a0.age);
    }

    /*Проверка BD_change*/
    printf("\nBD_change[1,5]:\n");

    a1.kod=1000;
    a1.name[0]='c';a1.name[1]='h';a1.name[2]='a';a1.name[3]='n';a1.name[4]='g';a1.name[5]='e';a1.name[6]='\0';
    a1.age=500;

    rez_in = BD_change(filename,&a1,sizeof(a1),1);
    rez_in = BD_change(filename,&a1,sizeof(a1),5);
    while(rez_out=BD_output(filename,&a0,sizeof(a0))){
        printf("%i. kod: %i | name: %s | age: %i\n",rez_out-1,a0.kod,a0.name,a0.age);
    }

    /*Проверка BD_remove*/
    printf("\nBD_remove[2,6]:\n");
    rez_in = BD_remove(filename,sizeof(a1),3);
    rez_in = BD_remove(filename,sizeof(a1),6);
    while(rez_out=BD_output(filename,&a0,sizeof(a0))){
        if(a0.kod != -1) printf("%i. kod: %i | name: %s | age: %i\n",rez_out-1,a0.kod,a0.name,a0.age);
    }

    /*Проверка BD_remove*/
    printf("\nSearch_by_code[3,6]: ");

    search_by_code(filename,&a0,sizeof(a0),c1.kod);
    if(a0.kod == c1.kod){ printf("[OK] "); }
    else{ printf("[Err]"); }

    search_by_code(filename,&a0,sizeof(a0),1000);
    if(a0.kod == 1000){ printf("[OK] \n"); }
    else{ printf("[Err] \n"); }


}

int main()
{
    test(); /*проверка корректной работы функций*/
    BD_check(); /*Проверка наличия БД*/
    printf("\n\nManagement-and-sale-of-supplies:\n"
"Данное приложение преднозначено для работы с БД реализованной на Си в виде бинарных файлов\n"
"БД сконфигурированна для ведения бухгалтерского учета в рамках небольшой компании\n"
"Можно: Принимать сотрудников, создавать контрагентов, добавлять склады, принимать поставки от контрагентов и реализовывать товар.\n");

    Menu();
    return 0;
}

/*Интерфейс приложения*/
void Menu(){
    char rez;

    do{
        printf("\nМеню:\n"
               "1. Добавить запись в таблицу\n"
               "2. Изменить запись в таблице\n"
               "3. Удалить запись в таблице\n"
               "4. Вывести все записи таблицы\n"
               "5. Проверить БД на ошибки\n"
               "0. Выход\n"
               "Выберите пункт меню: ");

        rez=getchar();while(getchar() != '\n'); /*Сбрасываем все символы кроме первого*/

        switch (rez) {
        case '1': add_record_to_table(); break;
        case '2': change_record_to_tabel(); break;
        case '3': remove_record_to_table(); break;
        case '4': out_record_to_table(); break;
        case '5': break;
        case '0': return;
        }
    }while(1);
    puts("\nВыход\n");
}

void add_record_to_table(){
    char rez;
    /*Вывод всех файлов*/
    puts("\nДобавление записи в таблицах:\nВсе таблицы:");
    for(int i=0;i<7;i++){
        printf("%i. %s\n",i+1,BD_list_fails[i]);
    }
    do{
        printf("\nВыберите файл [1-7]: ");
        rez=getchar();while(getchar() != '\n'); /*Сбрасываем все символы кроме первого*/

        switch (rez) {
        case '1': Add_Employees();break;
        case '2': Add_Nomenclature();break;
        case '3': Add_Stock();break;
        case '4': Add_Counterparties();break;
        case '5': Add_Management();break;
        case '6': Add_Deliveries();break;
        case '7': Add_Leftovers();break;
        case '0': return;
        }
    }while('1'>rez || rez>'7');
}

void Add_Employees(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<5;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&E_temp.code);break;
        case 1: printf("имя: "); scanf("%s",&E_temp.name); break;
        case 2: printf("возраст: "); scanf("%i",&E_temp.age);break;
        case 3: printf("должность: "); scanf("%s",&E_temp.position); break;
        case 4: printf("оклад: "); scanf("%i",&E_temp.salary); break;
        }
    }
    BD_add(BD_list_fails[0],&E_temp,E_size);
}


void Add_Nomenclature(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<3;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&N_temp.code);break;
        case 1: printf("наименование: "); scanf("%s",&N_temp.name);break;
        case 2: printf("процентную надбавку: "); scanf("%i",&N_temp.markup_percentage);break;
        }
    }
    BD_add(BD_list_fails[1],&N_temp,N_size);
}


void Add_Stock(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<3;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&S_temp.code); break;
        case 1: printf("наименование: "); scanf("%s",&S_temp.name);break;
        case 2: printf("вид склада: "); scanf("%s",&S_temp.type_of_warehouse);break;
        }
    }
    BD_add(BD_list_fails[2],&S_temp,S_size);
}


void Add_Counterparties(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<2;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&C_temp.code);break;
        case 1: printf("наименование: "); scanf("%s",&C_temp.name);break;
        }
    }
    BD_add(BD_list_fails[3],&C_temp,C_size);
}


void Add_Management(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<4;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&M_temp.code);break;
        case 1: printf("дата: "); scanf("%i",&M_temp.data); break;
        case 2: printf("код сотрудника: "); scanf("%i",&M_temp.code_employee); break;
        case 3: printf("статус сотрудника: "); scanf("%i",&M_temp.status);  break;
        }
    }
    BD_add(BD_list_fails[4],&M_temp,M_size);
}


void Add_Deliveries(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<5;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&D_temp.code);break;
        case 1: printf("дата: "); scanf("%i",&D_temp.data); break;
        case 2: printf("код контрагента: "); scanf("%i",&D_temp.code_counterparties); break;
        case 3: printf("код склада: "); scanf("%i",&D_temp.code_stock);break;
        case 4: printf("сумма: "); scanf("%i",&D_temp.summa);break;
        }
    }
    BD_add(BD_list_fails[5],&D_temp,D_size);
}


void Add_Leftovers(){
    printf("\nДобавление записи в таблицу:\n");
    for(int i=0;i<5;i++){
        printf("Введите ");
        switch (i) {
        case 0: printf("код: "); scanf("%i",&L_temp.code);break;
        case 1: printf("код номенклатуры: "); scanf("%i",&L_temp.code_nomenclature);break;
        case 2: printf("код склада: "); scanf("%i",&L_temp.code_stock);break;
        case 3: printf("количество: "); scanf("%i",&L_temp.quantity);break;
        case 4: printf("сумма: "); scanf("%i",&L_temp.summa);break;
        }
    }
    BD_add(BD_list_fails[6],&L_temp,L_size);
}

void change_record_to_tabel(){
    char rez;
    int rez_out, count;
    printf("\nИзменение данных в таблице:\nТаблица для изменения:\n");
    printf("1. %s\n2. %s\n6. %s\n",BD_list_fails[0],BD_list_fails[1],BD_list_fails[5]);

    do{
        printf("\nВыберите файл [1,2,6]: ");
        rez=getchar();while(getchar() != '\n'); /*Сбрасываем все символы кроме первого*/

        switch (rez) {
        case '1':
            count=0;
            printf("\n|Номер|Код|Наименование|Возраст|Должность|Оклад|\n");
            while(rez_out=BD_output(BD_list_fails[0],&E_temp,E_size)){ count++;
                if(E_temp.code != -1) printf("|%5i|%3i|%12s|%7i|%9s|%5i|\n",rez_out,E_temp.code,E_temp.name,E_temp.age,E_temp.position,E_temp.salary);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            Change_Employees(rez_out-1);
            break;
        case '2':
            count=0;
            printf("\n|Номер|Код|Наименование| % |\n");
            while(rez_out=BD_output(BD_list_fails[1],&N_temp,N_size)){ count++;
                if(N_temp.code != -1) printf("|%5i|%3i|%12s|%3i|\n",rez_out,N_temp.code,N_temp.name,N_temp.markup_percentage);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            Change_Nomenclature(rez_out-1);
            break;
        case '6':
            count=0;
            printf("\n|Номер|Код| Дата |Контрагент|Склад|Сумма|\n");
            while(rez_out=BD_output(BD_list_fails[5],&D_temp,D_size)){ count++;
                if(D_temp.code != -1) printf("|%5i|%3i|%6i|%10i|%5i|%5i|\n",rez_out,D_temp.code,D_temp.data,D_temp.code_counterparties,D_temp.code_stock,D_temp.summa);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            Change_Deliveries(rez_out-1);
            break;
        case '0': return;
        }
    }while('1'!=rez && rez!='2' && rez!='6');
}

void Change_Employees(int rez_out){
    BD_out(BD_list_fails[0],&E_temp,E_size,rez_out);
    printf("\nИзменение записи в таблице:\n");
    for(int i=0;i<2;i++){
        printf("Измените данные ");
        switch (i) {
        case 0: printf("должности: "); scanf("%s",&E_temp.position);break;
        case 1: printf("оклада: "); scanf("%i",&E_temp.salary);break;
        }
    }
    BD_change(BD_list_fails[0],&E_temp,E_size,rez_out);
}

void Change_Nomenclature(int rez_out){
    BD_out(BD_list_fails[1],&N_temp,N_size,rez_out);
    printf("\nИзменение записи в таблице:\n");
    for(int i=0;i<2;i++){
        printf("Измените данные ");
        switch (i) {
        case 0: printf("наименования: "); scanf("%s",&N_temp.name);break;
        case 1: printf("надбавки в процентах: "); scanf("%i",&N_temp.markup_percentage);break;
        }
    }
    BD_change(BD_list_fails[1],&N_temp,N_size,rez_out);
}

void Change_Deliveries(int rez_out){
    BD_out(BD_list_fails[5],&D_temp,D_size,rez_out);
    printf("\nИзменение записи в таблице:\n");
    for(int i=0;i<4;i++){
        printf("Измените данные ");
        switch (i) {
        case 0: printf("даты: "); scanf("%i",&D_temp.data);break;
        case 1: printf("кода контрагента: "); scanf("%i",&D_temp.code_counterparties);break;
        case 2: printf("кода склада: "); scanf("%i",&D_temp.code_stock);break;
        case 3: printf("суммы: "); scanf("%i",&D_temp.summa);break;
        }
    }
    BD_change(BD_list_fails[5],&D_temp,D_size,rez_out);
}

void remove_record_to_table(){
    char rez;
    int rez_out, count;
    printf("\nУдаление данных в таблице:\nДоступные таблицы:\n");
    printf("1. %s\n2. %s\n4. %s\n5. %s",BD_list_fails[0],BD_list_fails[1],BD_list_fails[3],BD_list_fails[4]);

    do{
        printf("\nВыберите файл [1,2,4,5]: ");
        rez=getchar();while(getchar() != '\n'); /*Сбрасываем все символы кроме первого*/

        switch (rez) {
        case '1':
            count=0;
            printf("\n|Номер|Код|Наименование|Возраст|Должность|Оклад|\n");
            while(rez_out=BD_output(BD_list_fails[0],&E_temp,E_size)){ count++;
                if(E_temp.code != -1) printf("|%5i|%3i|%12s|%7i|%9s|%5i|\n",rez_out,E_temp.code,E_temp.name,E_temp.age,E_temp.position,E_temp.salary);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            BD_remove(BD_list_fails[0],E_size,rez_out-1);
            break;
        case '2':
            count=0;
            printf("\n|Номер|Код|Наименование| % |\n");
            while(rez_out=BD_output(BD_list_fails[1],&N_temp,N_size)){ count++;
                if(N_temp.code != -1) printf("|%5i|%3i|%12s|%3i|\n",rez_out,N_temp.code,N_temp.name,N_temp.markup_percentage);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            BD_remove(BD_list_fails[1],N_size,rez_out-1);
            break;
        case '4':
            count=0;
            printf("\n|Номер|Код|Наименование|\n");
            while(rez_out=BD_output(BD_list_fails[3],&C_temp,C_size)){ count++;
                if(C_temp.code != -1) printf("|%5i|%3i|%12s|\n",rez_out,C_temp.code,C_temp.name);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            BD_remove(BD_list_fails[3],C_size,rez_out-1);
            break;
        case '5':
            count=0;
            printf("\n|Номер|Код| Дата |Работник|Статус|\n");
            while(rez_out=BD_output(BD_list_fails[4],&M_temp,M_size)){ count++;
                if(M_temp.code != -1) printf("|%5i|%3i|%6i|%8i|%6i|\n",rez_out,M_temp.code,M_temp.data,M_temp.code_employee,M_temp.status);
            }
            if(!count) {printf("Нет записей\n"); return;}
            printf("Выберите номер строки: "); scanf("%i",&rez_out);
            BD_remove(BD_list_fails[4],M_size,rez_out-1);
            break;
        case '0': return;
        }
    }while('1'!=rez && rez!='2' && rez!='4' && rez!='5');
}

void out_record_to_table(){
    char rez;
    int rez_out, count=0;
    /*Вывод всех файлов*/
    puts("\nВывод всех записей в таблицах:\nВсе таблицы:");
    for(int i=0;i<7;i++){
        printf("%i. %s\n",i+1,BD_list_fails[i]);
    }
    do{
        printf("\nВыберите файл [1-7]: ");
        rez=getchar();while(getchar() != '\n'); /*Сбрасываем все символы кроме первого*/

        switch (rez) {
        case '1':
            printf("\n|Номер|Код|Наименование|Возраст|Должность|Оклад|\n");
            while(rez_out=BD_output(BD_list_fails[0],&E_temp,E_size)){ count++;
                if(E_temp.code != -1) printf("|%5i|%3i|%12s|%7i|%9s|%5i|\n",rez_out,E_temp.code,E_temp.name,E_temp.age,E_temp.position,E_temp.salary);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '2':
            printf("\n|Номер|Код|Наименование| % |\n");
            while(rez_out=BD_output(BD_list_fails[1],&N_temp,N_size)){ count++;
                if(N_temp.code != -1) printf("|%5i|%3i|%12s|%3i|\n",rez_out,N_temp.code,N_temp.name,N_temp.markup_percentage);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '3':
            printf("\n|Номер|Код|Наименование|Вид склада|\n");
            while(rez_out=BD_output(BD_list_fails[2],&S_temp,S_size)){ count++;
                if(S_temp.code != -1) printf("|%5i|%3i|%12s|%10s|\n",rez_out,S_temp.code,S_temp.name,S_temp.type_of_warehouse);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '4':
            printf("\n|Номер|Код|Наименование|\n");
            while(rez_out=BD_output(BD_list_fails[3],&C_temp,C_size)){ count++;
                if(C_temp.code != -1) printf("|%5i|%3i|%12s|\n",rez_out,C_temp.code,C_temp.name);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '5':
            printf("\n|Номер|Код| Дата |Работник|Статус|\n");
            while(rez_out=BD_output(BD_list_fails[4],&M_temp,M_size)){ count++;
                if(M_temp.code != -1) printf("|%5i|%3i|%6i|%8i|%6i|\n",rez_out,M_temp.code,M_temp.data,M_temp.code_employee,M_temp.status);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '6':
            printf("\n|Номер|Код| Дата |Контрагент|Склад|Сумма|\n");
            while(rez_out=BD_output(BD_list_fails[5],&D_temp,D_size)){ count++;
                if(D_temp.code != -1) printf("|%5i|%3i|%6i|%10i|%5i|%5i|\n",rez_out,D_temp.code,D_temp.data,D_temp.code_counterparties,D_temp.code_stock,D_temp.summa);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '7':
            printf("\n|Номер|Код|Товар|Склад|Количество|Сумма|\n");
            while(rez_out=BD_output(BD_list_fails[6],&L_temp,L_size)){ count++;
                if(L_temp.code != -1) printf("|%5i|%3i|%5i|%5i|%10i|%5i|\n",rez_out,L_temp.code,L_temp.code_nomenclature,L_temp.code_stock,L_temp.quantity,L_temp.summa);
            }
            if(!count) {printf("Нет записей\n"); return;}
            break;
        case '0': return;
        }
    }while('1'>rez || rez>'7');
}



















