/*Работа с Базой Данных приложения Management-and-sale-of-supplies.
    - Содержит функции для Создания БД (пустой)
    - Функции: Добавления, Удаления, Изменения, Оптимизации БД*/

#include <stdio.h>

#define Number_of_files 2

#define ERROR /*error*/
#define Error_no_file 1
#define Error_damaged_file 2

/*Прототипы:*/
/*Запись и вывод данных файлов*/
int BD_write(char *file_name, char *mode, void *data, long *size_data, int index);
int BD_read(char *file_name, char *mode, void *data, long *size_data, int index);

/*Проверки БД*/
void BD_check();

/*Манипуляции с БД*/
int BD_add(char *filename, void *data, long *size_data);
int BD_change(char *filename, void *data, long *size_data, int index);
int BD_remove(char *filename, void *data, long *size_data, int index);
int BD_output(char *filename, void *data, long *size_data, void *fun);
int BD_out(char *filename, void *data, long *size_data, int index, void *fun);

/*Начальные данные*/
/*Список файлов БД*/
char *BD_list_fails[] = { "Employees.bin", "Nomenclature.bin" };


/*-----Функции ввода вывода--------------------------------------------------------------------*/
/*Записываем строчку данных в фаил*/
int BD_write(char *file_name, char *mode, void *data, long *size_data, int index=-1){
    char *c = (char *)data;

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return Error_no_file; }

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,index**size_data,SEEK_SET);

    /*Посимвольно заносим данные*/
    for(int i=0;i<*size_data;i++){
        putc(*c++,file);
    }

    fclose(file);
    return 0;
}

/*Читаем строчку данных в файле*/
int BD_read(char *file_name, char *mode, void *data, long *size_data, int index=-1){
    char *c = (char *)data; /*Указатель на структуру*/
    int simvol; /*Считеный символ*/

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return Error_no_file;}

    /*Устанавливаем указатель на нужной строчки*/
    if(index != -1) fseek(file,index**size_data,SEEK_SET);

    /*Посимволььно считываем данные*/
    for(int i=0;i<*size_data;i++){
        /*Проверка достигнут конец файла или файл поврежден*/
        if((simvol = getc(file))==EOF){ ERROR; fclose(file); return (i && i<size_data)?Error_damaged_file:EOF; }

        *c++ = simvol;
    }

    fclose(file);

    return 0;
}
/*---Функции тестирования и проверки----------------------------------------------------------------------*/
/*Проверят наличия бинарных файлов БД, если ненаходит, то создает заного пустой фаил*/
void BD_check(){
    /*Перебираем файлы БД*/
    for(int i=0;i<Number_of_files;i++){
        /*Если файл есть, то ничего не происходит
          Если файла нет, то он создается*/
        printf("%s\n",BD_list_fails[i]);
        fopen(BD_list_fails[i],"ab");
    }
}

/*-----Функции для работы с БД--------------------------------------------------------------------*/

/*Добавляем запись в файл БД*/
int BD_add(char *filename, void *data, long *size_data){
    return BD_write(filename,"ab",data,size_data);
}

/*Изменяет запись в файле БД*/
int BD_change(char *filename, void *data, long *size_data, int index){
    return BD_write(filename,"r+b",data,size_data,index);
}

/*Удаляет запись в файле БД*/
int BD_remove(char *filename, void *data, long *size_data, int index){
    data->kod=-1; /*Если код установлен в "-1", то эти данные удалены*/
    return BD_write(filename,"r+b",data,size_data,index);
}

/*Вывод всех записей из файла БД*/
int BD_output(char *filename, void *data, long *size_data, void *fun){
    int count=0;
    while( !BD_read(filename,"rb",data,size_data,count) ){
        /*Выводим данные*/
        fun(data,count);
        count++;
    }
    return count?1:0;
}

/*Вывод записи из файла БД*/
int BD_out(char *filename, void *data, long *size_data, int index, void *fun){
    if(BD_read(filename,"rb",data,size_data,index)){ERROR;return 1;}
    fun(data,index);
    return 0;
}



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/



































