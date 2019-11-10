/*Структуры Базы данных*/
/*То что тут описанно не рационально, можно было ограничится одной структурой или найти другой подход,
но так красиво*/

/*Справочники:*/
struct Employees{ /*Справочник сотрудников*/
    int code;
    char name[50];
    int age;
    char position[50];
    int salary;
}E_temp;

struct Nomenclature{ /*Номеклатура (Товар)*/
    int code;
    char name[50];
    int markup_percentage;
}N_temp;

struct Stock{ /*Склады*/
    int code;
    char name[50];
    char type_of_warehouse[50];
}S_temp;

struct Counterparties{ /*Контрагенты*/
    int code;
    char name[50];
}C_temp;

/*ригистры сведеней*/
struct Management{ /*Данные о перемещении сотрудников*/
    int code;
    int data;
    int code_employee;
    int status; /*Принят на работу(1)|Изменены данные(2)|Уволен(0)*/
}M_temp;

struct Deliveries{ /*Данное о остатках на складах*/
    int code;
    int data;
    int code_counterparties;
    int code_stock;
    int summa;
}D_temp;

/*регистры накоплений*/
struct Leftovers{ /*Считает сколько осталось на складе товара*/
    int code;
    int code_nomenclature;
    int code_stock;
    int quantity;
    int summa;
}L_temp;

/*Размеры структур (ширина таблиц)*/
long E_size=sizeof(E_temp), N_size=sizeof(N_temp), S_size=sizeof(S_temp), C_size=sizeof(C_temp);/*Справочники*/
long M_size=sizeof(M_temp), D_size=sizeof(D_temp);                                              /*регистры сведеней*/
long L_size=sizeof(L_temp);                                                                     /*регистры накоплений*/














