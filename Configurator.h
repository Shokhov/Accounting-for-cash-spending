/*Объявление и описание объектов Configurator.c*/

/*Справочники*/
extern struct Employees E_temp;        /*Справочник сотрудников*/
extern struct Nomenclature N_temp;     /*Номеклатура (Товар)*/
extern struct Stock S_temp;            /*Склады*/
extern struct Counterparties C_temp;   /*Контрагенты*/

/*регистры сведеней*/
extern struct Management M_temp; /*Данные о перемещении сотрудников*/
extern struct Deliveries D_temp; /*Данное о остатках на складах*/

/*регистры накоплений*/
extern struct Leftovers L_temp;  /*Считает сколько осталось на складе товара*/


/*Размеры структур (ширина таблиц)*/
extern long E_size, N_size, S_size, C_size;/*Справочники*/
extern long M_size, D_size;                /*регистры сведеней*/
extern long L_size;                        /*регистры накоплений*/