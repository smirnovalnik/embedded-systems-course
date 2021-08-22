/* Объявление и вызов функций */

/* Объявление функций */
тип-возвр-значения имя-функции(аргументы)
{
    объявления
    операторы
}

/* Вызов функций */
переменная = имя-функции(аргументы);


int add_two(int n)
{
    int tmp = 0;

    tmp = n + 2;
    return tmp;
}

a = add_two(2);



/* Объявление и вызов функций */

/* Объявление функций */
/* Абсолютное значение числа */
int abs(int n)
{
    if (n < 0)
        return -n;
    else
        return n;
}
/* Поиск максимального элемента в массиве */
int arr_max(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max)
            max = a[i];
    return max;
}


/* Включить светодиод */
void led_on()
{
    PORTC |= 0x01;
}

/* Вызов функций */
void main()
{
    int c;
    c = abs(-10); /* c = 10 */

    int arr[5] = {4, 3, 1, 7, 5};

    с = arr_max(arr, 5); /* с = 7 */
    led_on();
}



/* Объявление и вызов функций */

/* Объявление функций */
/* Обратить порядок элементов в массиве */
void reverse(char a[], int n)
{
    int i, j, tmp;
    for (i = 0, j = n - 1; i < j; i++, j--){
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
    return; /* Необязательно */
}

/* Вызов функций */
void main()
{
    char arr[5] = {4, 3, 1, 7, 5};
    char len = 5;

    reverse(arr, len);
    /* arr = {5, 7, 1, 3, 4} */
    int i;
    for (i = 0; i < len; i++){
        printf(“%d “, arr[i]);
    }
}



/* Указатели */

/* Объявление переменных и массива */
int x = 1, y = 2, z[10];
/* Объявление указателя */
int *p;

p = &x;/* p - адрес переменной x
или p - указывает на x */

/* Разыменование *p */
y = *p; /* *p = x = 1 -> y = 1 */
*p = 0; /* x = 0 */

p = &z[0];/*p - указывает на z[0]*/
*p = 100;/* z[0] = 100 */



/* Массивы и указатели */

/* Объявление массива */
int a[10];
/* Объявление указателя */
int *pa;

pa = &a[0]; /* Указатель на a[0] */
pa = a; /* Эквивалентно pa = &a[0] */

/* a[i] эквивалентно *(pa + i) */
*pa = 0; /* a[0] = 0 */
*(pa + 2) = 2; /* a[2] = 2 */



/* Строки и символы */

/* Объявление и инициализация строки */
char msg[] = "Hello\r\n";
/* Объявление и инициализация символа */
char ch = 'a';



/* Структуры */

/* Объявление структуры */
struct point {
    int x;
    int y;
};

/* Объявление переменной pt1 типа point */
struct point pt1;

/* Инициализация полей структуры */
pt1.x = 22;
pt1.y = 7;

/* Инициализация структуры */
struct point pt2 = {-10, 0};



/* Объединения */

/* Объявление объединения */
union {
    int word;
    short hword[2];
    char byte[4];
} u;

u.word = 0x12345678;

short lo_hword = u.hword[0];/* lo_hword = 0x5678 */
char hi_byte = u.byte[3];/* hi_byte = 0x12 */



/* Константы */

/* Макроопределения */
#define MAXLEN 100
char msg[MAXLEN + 1];

#define TRUE 1
#define FALSE 0

if (res == TRUE)
    ...

/* Перечисления */
enum month {JAN = 1, FEB, MAR, APR,
MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};
/* FEB = 2, MAR = 3, ...*/

enum boolean {FALSE = 0, TRUE};/* TRUE = 1 */



/* Битовые операции */

/* Битовые поля */
struct {
    unsigned int enable : 1;
    unsigned int test: 1;
    unsigned int err_code: 3;
} state_flags;

state_flags.enable = 1;
state_flags.test = 0;
state_flags.err_code = 4;


/* Пример без битовых полей */
#define ENABLE_FLAG 0
#define TEST_FLAG   1
#define ERR_MASK    0x1C
#define ERR_SHIFT   2

unsigned int flags;

flags |= ENABLE_FLAG;/* Установить бит 0*/
flags &= ~TEST_FLAG;/* Сбросить бит 1 */

/* Сбросить код ошибки */
flags &= ~(ERR_MASK << ERR_SHIFT);
/* Установить новый код ошибки */
flags |= (4 & ERR_MASK) << ERR_SHIFT;



/* Препроцессор */

/* Макроопределения */
#define ARR_SIZE 10
#define TRUE 1
#define FALSE 0
#define STEP 100

/* Подключение файлов */
#include "adc.h"
#include <stdio.h>


#define DEBUG

/* Условная компиляция */
#ifdef DEBUG
...
#endif



/* Обращение к регистрам специальных функций */

#define PORTC     *((volatile unsigned int *)(0x400B8000))
...
PORTC = PORTC | 1;    /* Установить 1 в PC0 */

#include "MDR32Fx.h"
...
MDR_PORTC->RXTX = MDR_PORTC->RXTX | 1; /* Установить 1 в PC0 */

#include <MDR32F9Qx_port.h>
...
PORT_SetBits(MDR_PORTC, PORT_Pin_0); /* Установить 1 в PC0 */



/* Обработка прерываний в Cortex-M3 */

Файл startup_MDR32F9Qx.s:
...
DCD     Timer1_IRQHandler          ; IRQ14
DCD     ADC_IRQHandler             ; IRQ17
...

Файлы разработчика, например, main.c:
/* Обработка прерывания по Timer1 */
void Timer1_IRQHandler(void)
{
    ...
}
/* Обработка прерывания по ADC */
void ADC_IRQHandler(void)
{
    ...
}



/* Ассемблерные вставки */
/* Для IDE Keil uVision */
__asm void add(int x1, int x2, int x3)
{
    ADDS R0, R0, R1
    ADDS R0, R0, R2
    BX LR
}

int swap32(int i)
{
    int res;
    __asm {
        REVSH res, i
    }
    return res;
}

__asm("WFI"); /* Выполнение одной команды */
