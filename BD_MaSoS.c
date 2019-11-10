/*Функции для работы с Базой Данных приложения Management-and-sale-of-supplies.
    - Содержит функции для Создания БД (пустой)
    - Функции: Добавления, Удаления, Изменения, Оптимизации БД*/

#include <stdio.h>
#include <sys/stat.h> /*Работает только в Linux*/

#define Number_of_files 7

#define ERROR /*error*/
#define Error_no_file 1
#define Error_damaged_file 2

/*Прототипы:*/
/*Запись и вывод данных*/
int BD_write(char *file_name, char *mode, void *data, long size_data, int index);
int BD_read(char *file_name, char *mode, void *data, long size_data, int index);
int BD_output(char *filename, void *data, long size_data);
int BD_out(char *filename, void *data, long size_data, int index);

/*Проверки БД*/
void BD_check();
int new_code(char *filename, long size_line, int *code);

/*Манипуляции с БД*/
int BD_add(char *filename, void *data, long size_data);
int BD_change(char *filename, void *data, long size_data, int index);
int BD_remove(char *filename, long size_data, int index);
int search_by_code(char *filename, void *data, long size_data, int search_code);

/*Начальные данные*/
/*Список файлов БД*/
char *BD_list_fails[Number_of_files] = {
    "BD/Employees.bin","BD/Nomenclature.bin","BD/Stock.bin","BD/Counterparties.bin",
    "BD/Management.bin","BD/Deliveries.bin",
    "BD/Leftovers.bin"
};


/*-----Функции ввода вывода--------------------------------------------------------------------*/
/*Записываем строчку данных в фаил*/
int BD_write(char *file_name, char *mode, void *data, long size_data, int index){
    char *c = (char *)data;

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return Error_no_file; }

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,index*size_data,SEEK_SET);

    /*Посимвольно заносим данные*/
    for(int i=0;i<size_data;i++){
        putc(*c++,file);
    }

    fclose(file);
    return 0;
}

/*Читаем строчку данных в файле*/
int BD_read(char *file_name, char *mode, void *data, long size_data, int index){
    char *c = (char *)data; /*Указатель на структуру*/
    int simvol; /*Считеный символ*/

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return Error_no_file;}

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,index*size_data,SEEK_SET);

    /*Посимволььно считываем данные*/
    for(int i=0;i<size_data;i++){
        /*Проверка достигнут конец файла или файл поврежден*/
        if((simvol = getc(file))==EOF){ ERROR; fclose(file); return EOF; }

        *c++ = simvol;
    }

    fclose(file);
    return 0;
}

/*Вывод всех записей из файла БД
Как работает: при обращении счетчик увеличивается на 1 и становится ">0"
когда будет прочитанна последняя строчка, то счетчик обнуляется
Реализован по принципу генератора в Python*/
int BD_output(char *filename, void *data, long size_data){
    static int count=0; /*счетчик*/

    if( !BD_read(filename,"rb",data,size_data,count) ){
        return ++count;
    }
    count = 0;
    return count;
}

/*Вывод записи из файла БД*/
int BD_out(char *filename, void *data, long size_data, int index){
    return BD_read(filename,"rb",data,size_data,index);
}

/*---Функции тестирования и проверки----------------------------------------------------------------------*/
/*Проверят наличия бинарных файлов БД, если ненаходит, то создает заного пустой фаил*/
void BD_check(){
    /*Создаем папку BD*/
    mkdir("BD", 0777);

    /*Перебираем файлы БД*/
    for(int i=0;i<Number_of_files;i++){
        /*Если файл есть, то ничего не происходит
          Если файла нет, то он создается*/
        fopen(BD_list_fails[i],"ab"); /*эта запись вообще правильная?*/
    }
}

/*Возвращает новый код*/
int new_code(char *filename, long size_line, int *code){
    int size_code=sizeof(int);
    int simvol;
    char *c=(char *)&code;

    FILE *file;
    if((file=fopen(filename,"rb"))==NULL) return Error_no_file;

    /*читаем код записей начиная с конца*/
    for(int index=1;;index++){
        fseek(file,-size_line*index+1,SEEK_END);
        for(int i=0;i<size_code;i++){
            if((simvol=getc(file))==EOF){return Error_damaged_file;}
            *c++ = simvol;
        }

        if(*code>0) break;
    }
    fclose(file);
    return 0;
}

/*-----Функции для работы с БД--------------------------------------------------------------------*/

/*Добавляем запись в файл БД*/
int BD_add(char *filename, void *data, long size_data){
    return BD_write(filename,"ab",data,size_data,-1);
}

/*Изменяет запись в файле БД*/
int BD_change(char *filename, void *data, long size_data, int index){
    return BD_write(filename,"r+b",data,size_data,index);
}

/*Удаляет запись в файле БД*/
int BD_remove(char *filename, long size_data, int index){
    int code=-1; /*Если код равен "-1", то запись удалена*/
    int size_code = sizeof(code);
    char *c = (char *)&code;

    FILE *file=NULL;
    if((file = fopen(filename,"r+b"))==NULL){ ERROR; return Error_no_file; }
    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,index*size_data,SEEK_SET);
    /*Посимвольно заносим данные*/
    for(int i=0;i<size_code;i++){
        putc(*c++,file);
    }
    fclose(file);
    return 0;
}

/*Ищем запись по коду*/
int search_by_code(char *filename, void *data, long size_data, int search_code){
    int code;
    long size_code = sizeof(code);
    int simvol;
    char *c = NULL;

    FILE *file=NULL;
    if((file = fopen(filename,"rb"))==NULL){ ERROR; return Error_no_file; }

    /*перебираем весь фаил*/
    while(1){
        c = (char *)&code;
        /*Читаем только код*/
        for(int i=0;i<size_code;i++){
            if((simvol=fgetc(file))==EOF){ fclose(file); return -1; }
            *c++ = simvol;
        }

        /*Если найден код записываем строчку данных*/
        if(search_code == code){
            c = (char *)data;
            fseek(file,-size_code,SEEK_CUR);
            for(int i=0;i<size_data;i++){
                if((simvol=fgetc(file))==EOF){ fclose(file); return 1; }
                *c++ = simvol;
            }
            break;
        }
        fseek(file,size_data-size_code,SEEK_CUR);
    }
    fclose(file);

    return 0;
}



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



































