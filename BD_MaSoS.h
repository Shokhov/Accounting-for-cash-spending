/*Объявление и описание объектов BD_MaSoS.c*/

/*Проверки БД*/
extern void BD_check();  /*Проверка наличия БД, если нет, то заного создаем*/

/*Манипуляции с БД*/
extern int BD_add(char *filename, void *data, long *size_data);                        /*Добавить запись*/
extern int BD_change(char *filename, void *data, long *size_data, int index);          /*Изменить запись*/
extern int BD_remove(char *filename, void *data, long *size_data, int index);          /*Удалить запись*/
extern int BD_output(char *filename, void *data, long *size_data, void *fun);          /*Вывести все записи*/
extern int BD_out(char *filename, void *data, long *size_data, int index, void *fun);  /*Вывести запись*/
