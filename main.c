/*Accounting-for-cash-spending*/
#include <stdio.h>
#include <stdlib.h>

//Функции для работы с базой данных
#include "BD_ACS.h"
#include "BD_ACS.c"

/*Прототипы*/
int display_all_records();

int main()
{
    if(BD_check()==3){return Error;}

    if(BD_out(BD_file,&Info_temp,0,size_info,-1)==2){return Error;}
    printf("number of records: %ld\namount: %ld\naverage amount: %ld\nlimit: %d\n",Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);

    display_all_records();

    for(int i=0;i<5;i++){
        Table_temp.code = ++Info_temp.number_of_records;
        Table_temp.amount = 123*(i*7)%2000;
        Table_temp.average_amount = (Info_temp.average_amount = (Info_temp.amount += Table_temp.amount)/Info_temp.number_of_records);
        Table_temp.limit_exceeded = 48+(Table_temp.amount>Info_temp.limit);
        for(int j=0;j<10;j++){
            if(j==2 || j==5) {Table_temp.date[j]='.';continue;}
            Table_temp.date[j]=j+48;
        }
        Table_temp.date[10]='\0';
        //printf("|%3ld|%s|%7ld|%5ld|%5c|\n",Table_temp.code,Table_temp.date,Table_temp.amount,Table_temp.average_amount,Table_temp.limit_exceeded);
        BD_add(BD_file,&Table_temp,size_info,size_table);
    }

    display_all_records();

    BD_in_info(Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);
    printf("\nnumber of records: %ld\namount: %ld\naverage amount: %ld\nlimit: %d\n",Info_temp.number_of_records,Info_temp.amount,Info_temp.average_amount,Info_temp.limit);






    return OK;
}

/*Вывод всех записей*/
int display_all_records()
{
    int rez_out=0;
    printf("\n|Код|     Дата     | Сумма |Сред.|Лимит|\n");
    while(BD_output(BD_file,&Table_temp,size_info,size_table)){
        printf("|%3ld|%s|%7ld|%5ld|%5c|\n",Table_temp.code,Table_temp.date,Table_temp.amount,Table_temp.average_amount,Table_temp.limit_exceeded);
        rez_out++;
    }
    if(!rez_out){printf("Нет записей\n");}
    return OK;
}










