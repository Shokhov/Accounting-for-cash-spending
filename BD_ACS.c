/*Функции для работы с Базой Данных приложения Accounting-for-cash-spending.
    - Создания/Проверка БД
    - Функции: Добавления, Изменения, Удаления, Поиска, Оптимизации БД*/

#include <stdio.h>
#include <sys/stat.h> /*Работает только в Linux*/

#define BD_file "BD/ACS.bin"
#define OK 0
#define Error_no_file 1
#define Error_damaged_file 2
#define Error 3

/*Прототипы:*/
/*Запись и вывод данных*/
int BD_write(char *file_name, char *mode, void *data, long size_info, long size_data, int index);
int BD_read(char *file_name, char *mode, void *data, long size_info, long size_data, int index);
int BD_output(char *filename, void *data, long size_info, long size_data);
int BD_out(char *filename, void *data, long size_info, long size_data, int index);

/*Проверки БД*/
int BD_check();

/*Манипуляции с БД*/
int BD_in_info(unsigned long number_of_records, unsigned long amount, unsigned long average_amount, unsigned short limit);
//int BD_add(char *filename, void *data, long size_data);
//int BD_change(char *filename, void *data, long size_data, int index);
//int BD_remove(char *filename, long size_data, int index);
//int search_by_code(char *filename, void *data, long size_data, int search_code);

/*Начальные данные*/
struct Info
{
    unsigned long number_of_records;
    unsigned long amount;
    unsigned long average_amount;
    unsigned short limit;
} Info_temp;
unsigned short size_info = (unsigned short)sizeof(Info_temp);

struct Table
{
    unsigned long code;
    char date[11];
    long amount;
    long average_amount;
    char limit_exceeded; /*0 or 1*/
} Table_temp;
unsigned short size_table = (unsigned short)sizeof(Table_temp);


/*-----Функции ввода/вывода--------------------------------------------------------------------*/
/*Записываем строчку данных в фаил*/
int BD_write(char *file_name, char *mode, void *data, long size_info, long size_data, int index){
    char *c = (char *)data;

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ return Error_no_file; }

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,size_info+index*size_data,SEEK_SET);

    /*Посимвольно заносим данные*/
    for(int i=0;i<size_data;i++){
        putc(*c++,file);
    }

    fclose(file);
    return OK;
}

/*Читаем строчку данных в файле*/
int BD_read(char *file_name, char *mode, void *data, long size_info, long size_data, int index){
    char *c = (char *)data; /*Указатель на структуру*/
    int simvol; /*Считеный символ*/

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ return Error_no_file; }

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,size_info+index*size_data,SEEK_SET);

    /*Посимволььно считываем данные*/
    for(int i=0;i<size_data;i++){
        /*Проверка достигнут конец файла или файл поврежден*/
        if((simvol = getc(file))==EOF){ fclose(file); return Error_damaged_file; }

        *c++ = simvol;
    }

    fclose(file);
    return OK;
}

/*Вывод всех записей из файла БД
Как работает: при обращении счетчик увеличивается на 1 и становится ">0"
когда будет прочитанна последняя строчка, то счетчик обнуляется
Реализован по принципу генератора в Python*/
int BD_output(char *filename, void *data, long size_info, long size_data){
    static int count=0; /*счетчик*/

    if( !BD_read(filename,"rb",data,size_info,size_data,count) ){
        return ++count;
    }
    count = 0;
    return count;
}

/*Вывод записи из файла БД*/
int BD_out(char *filename, void *data, long size_info, long size_data, int index){
    return BD_read(filename,"rb",data,size_info,size_data,index);
}

/*---Функции тестирования и проверки----------------------------------------------------------------------*/
/*Проверят наличия бинарных файлов БД, если ненаходит, то создает заного пустой фаил*/
int BD_check(){
    /*Создаем папку BD*/
    mkdir("BD", 0777);

    FILE *file=NULL;

    if((file=fopen(BD_file,"rb")) == NULL){
        fopen(BD_file,"wb");
        return BD_in_info(0,0,0,600);
    }

    fclose(file);
    return OK;
}

/*-----Функции для работы с БД--------------------------------------------------------------------*/
/*Ввод Info*/
int BD_in_info(unsigned long number_of_records,
               unsigned long amount,
               unsigned long average_amount,
               unsigned short limit)
{
    Info_temp.number_of_records=number_of_records;
    Info_temp.amount=amount;
    Info_temp.average_amount=average_amount;
    Info_temp.limit=limit;
    if(BD_write(BD_file,"r+b",&Info_temp,0,size_info,-1)){return Error;}
    return OK;
}


/*Добавляем запись в файл БД*/
int BD_add(char *filename, void *data, long size_info, long size_data){
    return BD_write(filename,"ab",data,size_info,size_data,-1);
}

/*Изменяет запись в файле БД*/
//int BD_change(char *filename, void *data, long size_data, int index){
//    return BD_write(filename,"r+b",data,size_data,index);
//}

/*Удаляет запись в файле БД*/
//int BD_remove(char *filename, long size_data, int index){
//    int code=-1; /*Если код равен "-1", то запись удалена*/
//    int size_code = sizeof(code);
//    char *c = (char *)&code;

//    FILE *file=NULL;
//    if((file = fopen(filename,"r+b"))==NULL){ ERROR; return Error_no_file; }
//    /*Устанавливаем указатель на нужной строчки*/
//    if(index != -1) fseek(file,index*size_data,SEEK_SET);
//    /*Посимвольно заносим данные*/
//    for(int i=0;i<size_code;i++){
//        putc(*c++,file);
//    }
//    fclose(file);
//    return 0;
//}

/*Ищем запись по коду*/
//int search_by_code(char *filename, void *data, long size_data, int search_code){
//    int code;
//    long size_code = sizeof(code);
//    int simvol;
//    char *c = NULL;

//    FILE *file=NULL;
//    if((file = fopen(filename,"rb"))==NULL){ ERROR; return Error_no_file; }

//    /*перебираем весь фаил*/
//    while(1){
//        c = (char *)&code;
//        /*Читаем только код*/
//        for(int i=0;i<size_code;i++){
//            if((simvol=fgetc(file))==EOF){ fclose(file); return -1; }
//            *c++ = simvol;
//        }

//        /*Если найден код записываем строчку данных*/
//        if(search_code == code){
//            c = (char *)data;
//            fseek(file,-size_code,SEEK_CUR);
//            for(int i=0;i<size_data;i++){
//                if((simvol=fgetc(file))==EOF){ fclose(file); return 1; }
//                *c++ = simvol;
//            }
//            break;
//        }
//        fseek(file,size_data-size_code,SEEK_CUR);
//    }
//    fclose(file);

//    return 0;
//}



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/


































