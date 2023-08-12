
/* Модель памяти */

int a;
int b = 42;
const char c = 87;
#define PI 3.1415926
void main()
{
    int d;
    char e[100];
    char f[] = "Hello";
    static int j;
    d = b + 2;
    float pi = PI;
    char *p = malloc(100);
}

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
