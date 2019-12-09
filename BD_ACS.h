/*Объявление и описание объектов BD_ACS.c*/

/*Запись и вывод данных*/
extern int BD_write(char *file_name, char *mode, void *data, long size_info, long size_data, int index);
extern int BD_read(char *file_name, char *mode, void *data, long size_info, long size_data, int index);
extern int BD_output(char *filename, void *data, long size_info, long size_data);
extern int BD_out(char *filename, void *data, long size_info, long size_data, int index);

/*Проверки БД*/
extern int BD_check();

/*Начальные данные*/
extern struct Info Info_temp;
extern struct Table Table_temp;
