#define RST_CLK_PER     *((volatile unsigned int *)(0x4002001C))
#define PORTC_RXTX      *((volatile unsigned int *)(0x400B8000))
#define PORTC_OE        *((volatile unsigned int *)(0x400B8004))
#define PORTC_ANALOG    *((volatile unsigned int *)(0x400B800C))
#define PORTC_PWR       *((volatile unsigned int *)(0x400B8018))

/* Функция main. Точка входа в программу */
int main(void)
{
    RST_CLK_PER = RST_CLK_PER | (1 << 23); /* Включаем тактирование порта C */

    PORTC_OE = PORTC_OE | 0x01; /* Настраиваем ножку 0 порта C на вывод */
    PORTC_ANALOG = PORTC_ANALOG | 0x01; /* Включаем цифровой режим работы ножки 0 */
    PORTC_PWR = PORTC_PWR | 0x02; /* Настраиваем мощность выходного буфера ножки 0 */

    for (;;)
    {
        PORTC_RXTX = PORTC_RXTX ^ 0x01; /* Инвертирование бита в регистре PORTC */
        for (int i = 0; i < 100000; i++); /* Программная задержка */
    }
}
