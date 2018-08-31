/* Имена объектов. Комментарии */

te+st/ = 1; /* Ошибка: имена должны содержать только буквы, цифры и символ _ */
1test = 3;  /* Ошибка: имена не должны начинаться с цифры */
test = 4;   /* ОК */

/* Test и test разные объекты - регистр имеет значение */
Test = 5;
test = 6;

TestLab1 = 7;         /* ОК CamelCase */
lesson_number_1 = 8;  /* ОК snake_case */
MPEI_ER_02_13 = 2;    /* ОК SCREAMING_SNAKE_CASE */
is_valid_parameter(first_param); /* ОК */

/* Многострочный комментарий по
стандарту C */
a = a + b; // Однострочный комментарий в стиле C++



/* Объявление переменных. Типы данных */

/* Объявление целочисленной
переменной со знаком
(дополнительный код со
знаком) */
int i;
i = 13;
/* Объявление беззнаковой
целочисленной переменной */
unsigned int j;
j = 0;
/* Объявление переменной
с плавающей запятой */
float a, b;
a = 10.0;
b = 20.0;
/* Объявление и инициализация
символьной переменной */
char c = 'a';



/* Оператор присваивания */

/* Объявление и инициализация */
int a = 1;
int b = 2;
int tmp;

/* Обмен значений a и b */
tmp = a;
a = b;
b = tmp; /* a = 2, b = 1 */

int i, j, k;

/* Множественное присваивание */
i = j = k = 0;



/* Арифметические операции */

int a = 0;
int b = 2;
int c;
/* Сложение */
c = a + b; /* c = 2 */
c = c + 2; /* c = 4 */
/* Вычитание */
c = b - a; /* c = 2 */
/* Умножение */
c = 4 * b; /* c = 8 */
/* Деление */
a = 10;
c = a / 2;   /* a = 5 */
c = a / 100; /* a = 0 */
/* Остаток от деления */
a = 13;
c = a % 10;  /* c = 3 */



/* Арифметические операции: деление */

int a;

/* Целочисленное деление */
a = 1 / 2;    /* a = 0 */
a = 10 / 100; /* a = 0 */
a = 3 / 2;    /* a = 1 */
a = 11 / 2;   /* a = 5 */

float c;

/* Деление чисел с плавающей запятой */
c = 1.0 / 2.0; /* c = 0.5 */
c = 1 / 2.0;   /* c = 0.5 */
c = 1.0 / 2;   /* c = 0.5 */
c = 1 / 2;     /* c = 0.0 */



/* Арифметические операции: сокращенная форма */

int i = 0;

i++; /* i = 1 */
++i; /* i = 2 */

/* Разница между префиксной и
постфиксной формой */
i = 0;
a = i++; /* a = 0, i = 1 */
a = ++i; /* a = 2, i = 2 */
i = 0;
i += 10; /* i = 10 */

a = 2;
i /= a + 3; /* i = 2 */




/* Битовые операции */

/* 0xА - шестнадцатеричная форма,
   012 - восьмеричная форма,
   10 - десятичная форма */

int a = 0x88;

a = a & 0xFE; /* Сброс бита a = 0x80 */

a = a | 0x02; /* Установка бита a = 0x82 */

a = a ^ 0x01; /* Инверсия бита a = 0x83 */
a = a ^ 0x01; /* Инверсия бита a = 0x82 */

a = ~a; /* Инверсия a = 0x7D */

a = 0x01;

a = a << 2; /* Сдвиг влево a = 0x04 */
a = a >> 1; /* Сдвиг вправо a = 0x02 */



/* Оператор ветвления */

/* Простая форма без {} */
if (выражение)
    оператор;


/* Простая форма с {} */
if (выражение) {
    оператор1;
    оператор2;
}

/* Проверка на четность */
if (a % 2 == 0)
    cnt++; // Одно утверждение


/* Простая форма с {} */
if (is_ready) {
    timeout = 100;
    status = OK;
}



/* Оператор ветвления */

/* Полная форма */
if (выражение) {
    оператор1;
}
else {
    оператор2;
}

if (выражение1) {
    оператор1;
}
else if (выражение2) {
    оператор2;
}
else {
    оператор3;
}

if (voltage < 100) {
    status = OK;
}
else {
    status = FAIL;
}

if (cmd == RUN) {
    run();
}
else if (cmd == STOP) {
    stop();
}
else {
    idle();
}




/* Операция сравнения и логические операции */

if (a < 0) ) {
    ...
}

if ((status & 0x01) == 0) {
    ...
}

if ((a > 0) && (a != 10)) {
    ...
}

if (!is_stopped) {
    ...
}



/* Оператор switch */

switch (выражение) {
    case константа1:
        оператор1;
        оператор2;
        break;
    case константа2:
        оператор3;
        break;
    case константа3:
    case константа4:
        оператор4;
        break;
    default: // необязательно
        оператор5;
}

switch (cmd) {
    case CMD_RUN:
        set_pwm(100);
        run();
        break;
    case CMD_STOP:
        stop();
        break;
    case CMD_IDLE:
    case CMD_RESET:
        idle();
        break;
    default:
        error();
}



/* Массивы */

/* Объявление массива без инициализации */
int arr[10];

arr[0] = 1;  /* Первый элемент */
arr[1] = 4;  /* Второй элемент */
arr[9] = 99; /* Последний элемент */

/* Компилятор не проверяет выход за пределы */
arr[-1] = 100; /* Ошибка при исполнении */
arr[10] = 100; /* Ошибка при исполнении */

/* Объявление массива с инициализацией */
char letters[] = {'x', 'y', 'z'};



/* Многомерные массивы */

int marr[10][10];

marr[0][0] = 13; /* Первый элемент */
marr[0][1] = 27;
marr[0][9] = 43;
marr[1][0] = 69;
marr[9][8] = 87;
marr[9][9] = 91; /* Последний элемент */



/* Операторы цикла */

/* Цикл while */
while (выражение) {
    оператор;
}

/* Цикл do-while */
do {
    оператор;
} while (выражение);

/* Цикл for */
for (выраж1; выраж2; выраж3) {
    оператор;
}


while (i < 10) {
    a[i] = i;
    i++;
}

do {
    spi_send_byte(cmd);
    status = spi_get_status();
} while (status != SPI_OK);

/* "Зануление" массива */
for (i = 0; i < n; i++) {
    a[i] = 0;
}

/* Бесконечный цикл */
for(;;);
while(1);



/* Операторы цикла*/

/* Оператор break */
while (выражение1) {
    ...
    if (выражение2)
        break;
    ...
}


/* Оператор continue */
while (выражение1) {
    ...
    if (выражение2)
        continue;
    ...
}


/* Проверка наличия элемента в массиве */
int is_found = 0, i = 0;
while (i < n) {
    if (a[i] == target) {
        is_found = 1;
        break;
    }
    i++;
}

/* Обработка только положительных элементов */
int i;
for (i = 0; i < n; i++) {
    if (a[i] < 0)
        continue;
    ...
}
