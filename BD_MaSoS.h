/*Объявление и описание объектов BD_MaSoS.c*/

/*Проверки БД*/
extern void BD_check();  /*Проверка наличия БД, если нет, то заного создаем*/
extern int new_code(char *filename, long size_line, int *code); /*получаем новый код*/

/*Манипуляции с БД*/
extern int BD_add(char *filename, void *data, long size_data);                 /*Добавить запись*/
extern int BD_change(char *filename, void *data, long size_data, int index);   /*Изменить запись*/
extern int BD_remove(char *filename, long size_data, int index);               /*Удалить запись*/

extern int search_by_code(char *filename, void *data, long size_data, int search_code); /*Поиск  записи по коду*/

/*Вывод*/
extern int BD_output(char *filename, void *data, long size_data);              /*Вывести все записи*/
extern int BD_out(char *filename, void *data, long size_data, int index);      /*Вывести запись*/

/*Начальные данные*/
extern char *BD_list_fails[]; /*Список файлов БД*/
