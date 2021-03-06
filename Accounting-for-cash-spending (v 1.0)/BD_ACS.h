/*Объявление и описание объектов BD_ACS.c*/

/*Запись и вывод данных*/
extern int BD_output(char *filename, void *data, long size_info, long size_data);
extern int BD_out(char *filename, void *data, long size_info, long size_data, int index);

/*Проверки БД*/
extern int BD_check();

/*Манипуляции с БД*/
extern int BD_in_info(unsigned long new_code, unsigned long number_of_records, double amount, double average_amount, unsigned short limit);
extern int BD_add(char *filename, void *data, long size_info, long size_data);
extern int BD_change(char *filename, void *data, long size_info, long size_data, int index);
extern int BD_remove(char *filename, long size_info, long size_data, int index);

/*Начальные данные*/
extern struct Info Info_temp;
extern struct Table Table_temp;
