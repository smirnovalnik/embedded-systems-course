/**
  ******************************************************************************
  * \file    uart_io.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.08.2017
  * \brief   Инициализация UART и перенаправление стандартного потока
  *          ввода/вывода на UART
  ******************************************************************************
*/

/* Подключение заголовочного файла с макроопределениями
   регистров специальных функций микроконтроллера К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32Fx.h>
/* Подключение заголовочного файла с конфигурацией библиотеки
   Standard Peripheral Library (SPL) для микроконтроллера К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32F9Qx_config.h>
/* Подключение функций управления портами из библиотеки SPL */
#include <MDR32F9Qx_port.h>
/* Подключение функций управления UART из библиотеки SPL */
#include <MDR32F9Qx_uart.h>
/* Подключение функций управления тактированием из библиотеки SPL */
#include <MDR32F9Qx_rst_clk.h>

/* Подключение заголовочного файла, содержащего платформо-зависимые
   макроопределения */
#include "uart_io.h"

/**
  * \brief  Инициализация интерфейса UART для стандартного потока ввода/вывода.
  * \note   Функция должна вызываться после настройки тактирования
  *         микроконтроллера и перед вызовом любой из функций для работы
  *         со стандартным потоком ввода/вывода.
  * \param  Нет
  * \retval Нет
  */
void uart_io_init(void)
{
    /* Инициализация ножек RX и TX */
    /* Разрешение тактирования портов с ножками RX и TX */
    RST_CLK_PCLKcmd(UART_IO_RX_PORT_CLK, ENABLE);
    RST_CLK_PCLKcmd(UART_IO_TX_PORT_CLK, ENABLE);

    /* Структура для настройки портов ввода/вывода, используемых в UART */
    PORT_InitTypeDef Port_InitStructure;

    /* Заполнение общих полей структуры Port_InitStructure */
    /* Подтяжка к питанию отключена */
    Port_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
    /* Подтяжка к нулю отключена */
    Port_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    /* Триггер Шмитта отключен */
    Port_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF;
    /* Режим работы выхода – управляемый драйвер */
    Port_InitStructure.PORT_PD = PORT_PD_DRIVER;
    /* Режим работы входного фильтра -  фильтр выключен */
    Port_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
    /* Переопределенная альтернативная функция порта */
    Port_InitStructure.PORT_FUNC = PORT_FUNC_OVERRID;
    /* Скорость нарастания/спада - максимально быстрый фронт */
    Port_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    /* Режим работы - цифровой вход/выход */
    Port_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

    /* Настройка UART RX на ввод */
    /* Настройка ножки порта на ввод */
    Port_InitStructure.PORT_OE = PORT_OE_IN;
    /* Указываем номер ножки, которую будет инициализировать */
    Port_InitStructure.PORT_Pin = UART_IO_RX_PIN;
    /* Инициализация ножки */
    PORT_Init(UART_IO_RX_PORT, &Port_InitStructure);

    /* Настройка UART TX на вывод, изменяем в структуре Port_InitStructure
       только различающиеся поля: ввод/вывод и номер ножки */
    /* Настройка ножки порта на вывод */
    Port_InitStructure.PORT_OE = PORT_OE_OUT;
    /* Указываем номер ножки, которую будет инициализировать */
    Port_InitStructure.PORT_Pin = UART_IO_TX_PIN;
    /* Инициализация ножки */
    PORT_Init(UART_IO_TX_PORT, &Port_InitStructure);

    /* Инициализация UART */

    /* Разрешение тактирования UART */
    RST_CLK_PCLKcmd(UART_IO_CLK, ENABLE);

    /* Настройка делителя тактовой частоты UART */
    UART_BRGInit(UART_IO, UART_HCLKdiv1);

    /* Структура для настройки UART */
    UART_InitTypeDef UART_InitStructure;

    /* Заполнение полей структуры UART_InitStructure */
    /* Скорость передачи */
    UART_InitStructure.UART_BaudRate                = UART_IO_BAUDRATE;
    /* Количество бит данных - 8 */
    UART_InitStructure.UART_WordLength              = UART_WordLength8b;
    /* Количество стоп бит - 1 */
    UART_InitStructure.UART_StopBits                = UART_StopBits1;
    /* Без контроля бита паритета */
    UART_InitStructure.UART_Parity                  = UART_Parity_No;
    /* Буфер FIFO отключен */
    UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
    /* Включаем передатчик и приемник */
    UART_InitStructure.UART_HardwareFlowControl     =
        UART_HardwareFlowControl_RXE |
        UART_HardwareFlowControl_TXE;

    /* Инициализация UART */
    UART_Init(UART_IO, &UART_InitStructure);

    /* Разрешение работы UART */
    UART_Cmd(UART_IO, ENABLE);
}

/**
  * \brief  Прочитать символ из UART
  * \note   Нет
  * \param  Нет
  * \retval Код символа, прочитанного из UART
  */
int stdin_getchar(void)
{
    /* Ожидание флага заполнения буфера приема (RXFF) */
    while (UART_GetFlagStatus(UART_IO, UART_FLAG_RXFF) != SET);

    /* Чтение данных из UART */
    return (int)UART_ReceiveData(UART_IO);
}

/**
  * \brief  Передать символ по UART
  * \note   Нет
  * \param  Код символа для передачи
  * \retval Код символа, переданного по UART
  */
int stdout_putchar(int ch)
{
    /* Ожидание флага освобождения буфера передачи (TXFE) */
    while (UART_GetFlagStatus(UART_IO, UART_FLAG_TXFE) != SET);

    /* Отправка данных по UART */
    UART_SendData(UART_IO, (char)ch);

    return ch;
}
