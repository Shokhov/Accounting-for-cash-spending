/*Работа с Базой Данных приложения Management-and-sale-of-supplies.
    - Содержит функции для Создания БД (пустой)
    - Функции: Добавления, Удаления, Изменения, Оптимизации БД*/

#include <stdio.h>

#define ERROR /*error*/
#define Number_of_files 2

/*Функции:*/
/*Запись и вывод данных файлов*/
int BD_write(char *file_name, char *mode, void *ptn, int size_ptn);
int BD_read(char *file_name, char *mode, void *ptn, int size_ptn);

void BD_check();

/*-------------------------------------------------------------------------*/
/*Записываем строчку данных в фаил*/
int BD_write(char *file_name, char *mode, void *ptn, int size_ptn){
    char *c = (char *)ptn;

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return 1; }

    /*Посимвольно заносим данные*/
    for(int i=0;i<size_ptn;i++){
        putc(*c++,file);
    }

    fclose(file);
    return 0;
}

/*Читаем строчку данных в файле*/
int BD_read(char *file_name, char *mode, void *ptn, int size_ptn){
    char *c = (char *)ptn; /*Указатель на структуру*/
    int simvol; /*Считеный символ*/

    FILE *file=NULL;
    if((file = fopen(file_name,mode))==NULL){ ERROR; return 1;}

    /*Посимволььно считываем данные*/
    for(int i=0;i<size_ptn;i++){
        if((simvol = getc(file))==EOF){ ERROR; break;}

        *c++ = simvol;
    }

    fclose(file);
    return 0;
}
/*-------------------------------------------------------------------------*/

/*Список файлов БД*/
char *BD_list_fails[] = { "Employees.bin", "Nomenclature.bin" };


/*Функция:
Получает: ничего
Делает: проверят наличия бинарных файлов БД, если ненаходит, то создает заного пустой фаил
Возвращает: ничего*/
void BD_check(){
    /*Перебираем файлы БД*/
    for(int i=0;i<Number_of_files;i++){
        /*Если файл есть, то ничего не происходит
          Если файла нет, то он создается*/
        printf("%s\n",BD_list_fails[i]);
        fopen(BD_list_fails[i],"ab");
    }
}

/*-------------------------------------------------------------------------*/
