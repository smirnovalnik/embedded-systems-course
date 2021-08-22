/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.1
  * \date    1.08.2017
  * \brief   Пример проекта для выполнения лабораторной работы №3.
  *          Программа генерирует гармонический сигнал (sin) частотой 50 Гц
  *          в диапазоне углов от alpha до beta на выходе X15 (DAC_OUT).
  *          Частота дискретизации сигнала 10 кГц. Контроль генерируемого
  *          сигнала производится с помощью осциллографа.
  *          Для генерации используется цифро-аналоговый преобразователь (ЦАП),
  *          контроллер прямого доступа к памяти (ПДП) и таймер.
  *          Углы гармонического сигнала задаются на персональном
  *          компьютере через эмулятор терминала. Углы задаются в градусах
  *          от 0 до 360.
  ******************************************************************************
*/

/* Подключение заголовочного файла с макроопределениями
   регистров специальных функций К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32Fx.h>
/* Подключение заголовочного файла с конфигурацией
   Standard Peripheral Library (SPL) для К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32F9Qx_config.h>
/* Подключение функций управления тактированием */
#include <MDR32F9Qx_rst_clk.h>
/* Подключение функций управления таймерами */
#include <MDR32F9Qx_timer.h>
/* Подключение функций управления портами */
#include <MDR32F9Qx_port.h>
/* Подключение функций управления ЦАП */
#include <MDR32F9Qx_dac.h>
/* Подключение функций управления контроллером ПДП */
#include <MDR32F9Qx_dma.h>

/* Подключение функций стандартного ввода/вывода (printf и scanf) */
#include <stdio.h>
/* Подключение макроопределений int8_t, uint8_t, int16_t и т.д. */
#include <stdint.h>
/* Подключение функций для работы со строками (strcmp) */
#include <string.h>

/* На отладочной плате модуль UART микроконтроллера подключен
   к преобразователю UART - RS-232.
   Персональный компьютер подключается к разъему RS-232 отладочной
   платы через преобразователь RS-232 - USB.
   На персональном компьютере в программе терминале выбирается
   COM порт с параметрами: 115200 8N1 (скорость передачи 115200 бод,
   8 бит данных, без контроля четности и 1 стоп бит).
   В данном проекте ввод и вывод символов производится
   с помощью стандартной библиотеки ввода/вывода (stdio.h).
   Для использования стандартной библиотеки ввода/вывода
   переопределены функции stdin_getchar и stdout_putchar
   для приема и передачи символов по UART (uart_io.h, uart_io.c).
*/
#include "uart_io.h"

/* Массив sin_arr с отсчетами гармонического сигнала */
#include "sin.h"

/*  Разъем X15 (DAC_OUT) подключен к ножке 0 порта E. Данная ножка в режиме
    альтернативной функции подключается к выходу второго канала ЦАП (DAC2).
    Контроллер прямого доступа к памяти (DMA) настроен на копирование данных
    из массива sin_arr (файл sin.h) в регистр ЦАП. Копирование производится
    по событию переполнения таймера 1 (Timer1) каждые 100 мкс (10 кГц).
    По окончанию копирования DMA вызывает подпрограмму обработчик прерывания
    (DMA_IRQHandler) из файла MDR32F9Qx_it.c (в файле собраны все обработчики
    прерывания). В подпрограмме обработчике производится перенастройка
    DMA контроллера. В качестве начального (a_idx) и конечного индекса (b_idx)
    массива sin_arr используются значения полученные при вводе в бесконечном
    цикле внутри main. Изменение индексов a_idx и b_idx позволяет менять
    углы выводимого гармонического сигнала.

                      a_idx                             b_idx
    sin_arr |a0 a1 ...aa......ai........................ab........ an|
                              ^                         ^
                   Timer1 -> DMA -> DAC2               DMA_IRQHandler

    Файлы sin.h и sin.c сгенерированы скриптом sin_gen.py
*/

/* Структура для инициализации порта */
PORT_InitTypeDef            PORT_InitStructure;
/* Структура для инициализации таймера */
TIMER_CntInitTypeDef        TIM_CntInit;
/* Структура для инициализации DMA */
DMA_ChannelInitTypeDef      DMA_InitStr;
/* Структура для инициализации конфигурации канала DMA */
DMA_CtrlDataInitTypeDef     DMA_PriCtrlStr;

/* Начальные и конечные индексы в массиве sin_arr */
volatile uint32_t a_idx = 0;
volatile uint32_t b_idx = SIN_LEN - 1;

/* Настройка тактового сигнала центрального процессора (ЦП) микроконтроллера.
   Частота ЦП: fhse * (1/div_hse) * pll_mul * (1/cpu_div) =
   8 МГц * (1/1) * 10 * (1/1) = 80 МГц
   */
void sys_clk_init(void)
{
    /* Разрешение работы внешнего высокочастотного резонатора (HSE) */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);

    /* Проверка готовности HSE */
    if (RST_CLK_HSEstatus() == SUCCESS)
    {
        /* Выбор резонатора HSE для тактирования ФАПЧ ЦП (PLL_CPU) и
           установка множителя частоты ФАПЧ на 10 */
        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
        /* Включение ФАПЧ */
        RST_CLK_CPU_PLLcmd(ENABLE);
        /* Проверка завершения инициализации ФАПЧ */
        if (RST_CLK_CPU_PLLstatus() == SUCCESS)
        {
            /* Установка предделителя тактового сигнала ЦП в 1 */
            RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
            /* Подключение выхода ФАПЧ к CPU_C3*/
            RST_CLK_CPU_PLLuse(ENABLE);
            /* Выбор тактового сигнала CPU_C3 для тактирования ЦП */
            RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
        }
    }
    else
    {
        /* Сюда программа не должна никогда попасть, а если попала, то значит
           аппаратные проблемы с внешним кварцевым резонатором */
        for(;;);
    }

    /* Обновление глобальной переменной SystemCoreClock, содержащей значение
       частоты центрального процессора. */
    SystemCoreClockUpdate();
}

/* Функция main. Точка входа в программу */
int main(void)
{
    /* Вызов функции настройки частоты тактирования ЦП */
    sys_clk_init();

    /* Вызов функции инициализации UART для стандартного потока */
    uart_io_init();

    /* Разрешение тактирования периферийных модулей */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK|
                    RST_CLK_PCLK_PORTE  |
                    RST_CLK_PCLK_TIMER1 |
                    RST_CLK_PCLK_DMA    |
                    RST_CLK_PCLK_DAC, ENABLE);

    /* Без включения тактирования модулей SSP1 и SSP2 модуль DMA не работает
       корректно */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2, ENABLE);

    /* Настройка портов ввода/вывода (GPIO) */
    /* Заполнение полей структуры PORT_InitStructure значениями по умолчанию */
    PORT_StructInit(&PORT_InitStructure);

    /* Подтяжка к питанию отключена */
    PORT_InitStructure.PORT_PULL_UP     = PORT_PULL_UP_OFF;
    /* Подтяжка к нулю отключена */
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    /* Настройка ножки порта на вывод */
    PORT_InitStructure.PORT_OE          = PORT_OE_OUT;
    /* Аналоговая функция порта */
    PORT_InitStructure.PORT_MODE        = PORT_MODE_ANALOG;
    /* Альтернативная функция порта */
    PORT_InitStructure.PORT_FUNC        = PORT_FUNC_ALTER;
    /* Указываем номер ножки, которую будет инициализировать */
    PORT_InitStructure.PORT_Pin         = PORT_Pin_0;

    /* Инициализация ножки */
    PORT_Init(MDR_PORTE, &PORT_InitStructure);

    /* Настройка ЦАП (DAC) */
    /* Опорный сигнал ЦАП 2 - напряжение на ножке AVсс */
    DAC2_Init(DAC2_AVCC);
    /* Разрешение работы ЦАП 2 */
    DAC2_Cmd(ENABLE);

    /* Настройка контроллера прямого доступа к памяти (DMA) */
    /* Сброс DMA модуля */
    DMA_DeInit();
    /* Заполнение полей структуры DMA_InitStr значениями по умолчанию */
    DMA_StructInit(&DMA_InitStr);

    /* Настройка первичной конфигурации канала DMA */
    /* Источник данных DMA - массив sin_arr */
    DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)sin_arr;
    /* Приемник данных DMA - ЦАП канал 2 */
    DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t)(&(MDR_DAC->DAC2_DATA));
    /* Увеличение адреса источника на 2 байта каждую пересылку */
    DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
    /* Не увеличиваем адрес приемника данных */
    DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
    /* Выравнивание данных по 2 байта */
    DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    /* Базовый режим работы */
    DMA_PriCtrlStr.DMA_Mode = DMA_Mode_Basic;
    /* Пересылаем все данные из массива sin_arr */
    DMA_PriCtrlStr.DMA_CycleSize = SIN_LEN;
    /* Однократная передача */
    DMA_PriCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
    DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
    DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;

    /* Настройка канала DMA */
    /* Указываем структуру DMA_PriCtrlStr как основную конфигурацию канала */
    DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
    DMA_InitStr.DMA_Priority = DMA_Priority_Default;
    DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
    DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

    /* Инициализация канала DMA_Channel_TIM1 (10 канал).
       Пересылка будет производится по событию таймера 1 */
    DMA_Init(DMA_Channel_TIM1, &DMA_InitStr);

    /* Разрешение работы канала DMA_Channel_TIM1 */
    DMA_Cmd(DMA_Channel_TIM1, ENABLE);

    /* Настройка таймера.
       Параметры таймера Timer1:
       HCLK = 80 MHz, Prescaler = 7 -> Clock = HCLK/(Prescaler + 1) = 10 MHz,
       Period = 999 -> Frequency = Clock/(Period + 1) = 10 kHz */
    /* Сброс таймера Timer1 */
    TIMER_DeInit(MDR_TIMER1);

    /* Разрешение тактирования Timer1 */
    TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1);

    /* Заполнение полей структуры TIM_CntInit значениями по умолчанию */
    TIMER_CntStructInit(&TIM_CntInit);

    /* Заполнение полей структуры TIM_CntInit */
    /* Настройка предделителя */
    TIM_CntInit.TIMER_Prescaler                = 7;
    /* Настройка периода */
    TIM_CntInit.TIMER_Period                   = 999;
    /* Источник тактирования - внутренний генератор */
    TIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
    /* Счет "вверх" */
    TIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;
    /* Немедленное обновление счетчика по переполнению */
    TIM_CntInit.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;

    /* Инициализация Timer1 */
    TIMER_CntInit(MDR_TIMER1, &TIM_CntInit);

    /* Разрешение запуска DMA по переполнению Timer1 */
    TIMER_DMACmd(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);

    /* Разрешение работы Timer1 */
    TIMER_Cmd(MDR_TIMER1, ENABLE);

    /* Настройка приоритета прерывания по DMA */
    NVIC_SetPriority(DMA_IRQn, 15);
    /* Разрешение прерывания по DMA */
    NVIC_EnableIRQ(DMA_IRQn);

    /* Печать строки приветствия в стандартный поток */
    printf("Generate sin in range 0 - 360 degrees\n");

    uint32_t alpha = 0;
    uint32_t beta = 360;

    /* Бесконечный цикл */
    for (;;)
    {
        printf("Input angles alpha, beta:\n");
        /* Ожидание ввода команды */
        if (scanf("%u %u", &alpha, &beta) == 2)
        {
            /* Проверка диапазона углов */
            if (alpha < beta && beta <= 360)
            {
                /* Вычисление индексов массива sin_arr */
                a_idx = (alpha * (SIN_LEN - 1)) / 360;
                b_idx = (beta * (SIN_LEN - 1)) / 360;
            }
            else
            {
                printf("Incorrect range\n"); /* Сообщаем о некорректном вводе */
            }
        }
        else
        {
            while(getchar() != '\r'); /* Очищаем буфер ввода */
            printf("Incorrect input\n"); /* Сообщаем о некорректном вводе */
        }
    }
}
