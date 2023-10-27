/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.10.2017
  * \brief   Пример проекта для выполнения лабораторной работы №4.
  *          Программа измеряет среднее значение сигнала, поданного
  *          на разъем X2(ADC). Частота дискретизации сигнала - 10 кГц.
  *          Измеренное среднее значение напряжения выводится
  *          на семисегментный индикатор на основе драйвера MAX7219
  *          каждые 100 мс. Формат вывода: x.yyy – три значащих цифры
  *          после запятой. Источником сигнала является генератор
  *          импульсов произвольной формы. Для измерения используется
  *          аналого-цифровой преобразователь (АЦП), контроллер прямого доступа
  *          к памяти (ПДП) и таймер.
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
/* Подключение функций управления портами */
#include <MDR32F9Qx_port.h>
/* Подключение функций управления таймерами */
#include <MDR32F9Qx_timer.h>
/* Подключение функций управления АЦП */
#include <MDR32F9Qx_adc.h>
/* Подключение функций управления контроллера ПДП */
#include <MDR32F9Qx_dma.h>

#include "systick.h"

/* Подключение индикаторов на основе MAX7219 к отладочной плате.
   Индикатор | Отладочная плата | Микроконтроллер | Назначение
   VCC       | XP13:28(X26:28)  |       -         | +5В
   GND       | XP13:30(X26:30)  |       -         | GND
   DIN       | XP13:11(X26:11)  | PD6 (SSP2_TXD)  | SPI_MOSI
   CS        | XP13:24(X26:24)  | PC0 (SSP2_FSS)  | SPI_NSS
   CLK       | XP13:10(X26:10)  | PD5 (SSP2_CLK)  | SPI_SCK
*/

/* Подключение драйвера семисегментного индикатора */
#include "max7219_7seg.h"

/* Структура для инициализации порта */
PORT_InitTypeDef            PORT_InitStructure;
/* Структура для инициализации таймера */
TIMER_CntInitTypeDef        TIM_CntInit;
/* Структура для инициализации АЦП */
ADC_InitTypeDef             ADC_InitStr;
/* Структура для инициализации каналов АЦП */
ADCx_InitTypeDef            ADCx_InitStr;
/* Структура для инициализации DMA */
DMA_ChannelInitTypeDef      DMA_InitStr;
/* Структура для инициализации первичной конфигурации канала DMA */
DMA_CtrlDataInitTypeDef     DMA_PriCtrlStr;

/*  Структура программы
    Разъем X2 (ADC) подключен к выводу микроконтроллера PD7, который в свою
    очередь внутренне подключен к седьмому каналу АЦП. Модуль АЦП используется
    в режиме однократного преобразования.
    Таймер настраивается на прерывание по переполнению каждые 100 мкс (10 кГц).
    В прерывании таймера производится запуск АЦП.
    Частота прерывания таймера определяет частоту дискретизации сигнала.
    Модуль прямого доступа к памяти настраивается на копирование результата АЦП
    в буфер по флагу окончания преобразования АЦП. Копирование производится
    в два буфера по 512 отсчетов. После того, как заполнился первый буфер
    (произошло 512 событий окончания преобразования и столько же копирований
    из АЦП в память) вызывается прерывание ПДП, в котором устанавливается флаг
    готовности первого буфера и ПДП перенастраивается на второй буфер.
    0                          N-1 0                           N-1
    -------------------------------------------------------------
    |            Буфер 1          |            Буфер 2          |
    -------------------------------------------------------------
      ^                           ^
      Таймер->АЦП->ПДП(пересылка) ПДП(окончание пересылки)->Флаг готовности,Перенастройка

    Организация копирования в два буфера позволяет выполнять обработку
    результата преобразования не задерживая накопление следующих данных.

    В основном цикле программы по флагам готовности буферов производится
    подсчет среднего значения кода АЦП.
    Также в основном цикле производится вывод информации на семисегментный
    индикатор. Перед выводом код АЦП преобразуется в напряжение.
    Драйвер индикатора находится в файлах max7219_7seg.h и max7219_7seg.c
    Семисегментный индикатор подключен по интерфейсу SPI.
    В драйвере производится первичная инициализация MAX7219: включается
    интегральная схема, выключается тестовый режим, снижается яркость,
    включается режим отображения 4 символов и режим дешифрации цифр.
    Подробная информация о драйвере индикатора MAX7219 указана в документации.
    Информация на индикатор выводится каждые 100 мс.
*/

/* Размер буфера АЦП */
#define ADC_BUF_SIZE    512

/* Буферы АЦП */
int16_t adc_buf1[ADC_BUF_SIZE];
int16_t adc_buf2[ADC_BUF_SIZE];

/* Флаги готовности буферов */
volatile uint8_t adc_buf1_rdy = 0;
volatile uint8_t adc_buf2_rdy = 0;

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

    /* Разрешение тактирования периферийных модулей */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK  |
                    RST_CLK_PCLK_PORTD  |
                    RST_CLK_PCLK_TIMER1 |
                    RST_CLK_PCLK_DMA    |
                    RST_CLK_PCLK_ADC , ENABLE);

    /* Без включения тактирования модулей SSP1 и SSP2
       модуль DMA не работает корректно */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2, ENABLE);


    /* Настройка портов ввода/вывода (GPIO) */
    /* Заполнение полей структуры PORT_InitStructure значениями по умолчанию */
    PORT_StructInit(&PORT_InitStructure);

    /* Настройка входа АЦП X2 -> PD7 -> ADC_CH7 */
    /* Настройка ножки порта на ввод */
    PORT_InitStructure.PORT_OE          = PORT_OE_IN;
    /* Аналоговая функция порта */
    PORT_InitStructure.PORT_MODE        = PORT_MODE_ANALOG;
    /* Указываем номер ножки, которую будет инициализировать */
    PORT_InitStructure.PORT_Pin         = PORT_Pin_7;

    /* Инициализация ножки */
    PORT_Init(MDR_PORTD, &PORT_InitStructure);


    /* Настройка контроллера прямого доступа к памяти (DMA) */
    /* Сброс DMA модуля */
    DMA_DeInit();

    /* Заполнение полей структуры DMA_InitStr значениями по умолчанию */
    DMA_StructInit(&DMA_InitStr);

    /* Настройка первичной конфигурации канала DMA */
    DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)(&(MDR_ADC->ADC1_RESULT));
    DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t)adc_buf1; /* Первый буфер */
    DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncNo;
    DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncHalfword;
    DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_PriCtrlStr.DMA_Mode = DMA_Mode_Basic;
    DMA_PriCtrlStr.DMA_CycleSize = ADC_BUF_SIZE;
    DMA_PriCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
    DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
    DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;

    /* Настройка конфигурации канала DMA */
    DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
    DMA_InitStr.DMA_Priority = DMA_Priority_Default;
    DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
    DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

    /* Инициализация канала DMA_Channel_ADC1 (8 канал) */
    DMA_Init(DMA_Channel_ADC1, &DMA_InitStr);

    /* Разрешение работы канала DMA_Channel_ADC1 */
    DMA_Cmd(DMA_Channel_ADC1, ENABLE);


    /* Настройка таймера.
       Параметры таймера Timer1:
       HCLK = 80 MHz, Prescaler = 7 -> Clock = HCLK/(Prescaler + 1) = 10 MHz,
       Period = 999 -> Frequency = Clock/(Period + 1) = 10 kHz */
    /* Сброс Timer 1*/
    TIMER_DeInit(MDR_TIMER1);

    /* Разрешение тактирования Timer1 */
    TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1);

    /* Заполнение полей структуры TIM_CntInit значениями по умолчанию */
    TIMER_CntStructInit(&TIM_CntInit);

    /* Заполнение полей структуры TIM_CntInit */
    TIM_CntInit.TIMER_Prescaler                = 7;
    TIM_CntInit.TIMER_Period                   = 999;
    TIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
    TIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;

    /* Инициализация Timer1 */
    TIMER_CntInit(MDR_TIMER1, &TIM_CntInit);
    /* Разрешение прерывания по переполнению Timer1 */
    TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);


    /* Настройка АЦП */
    /* Сброс ADC */
    ADC_DeInit();

    /* Заполнение полей структуры ADC_InitStr значениями по умолчанию */
    ADC_StructInit(&ADC_InitStr);

    ADC_InitStr.ADC_SynchronousMode      = ADC_SyncMode_Independent;
    ADC_InitStr.ADC_StartDelay           = 0;
    ADC_InitStr.ADC_TempSensor           = ADC_TEMP_SENSOR_Disable;
    ADC_InitStr.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Disable;
    ADC_InitStr.ADC_IntVRefTrimming      = 1;
    ADC_Init(&ADC_InitStr);

    /* Настройка канала АЦП */
    /* Заполнение полей структуры ADCx_InitStr значениями по умолчанию */
    ADCx_StructInit(&ADCx_InitStr);

    ADCx_InitStr.ADC_ClockSource      = ADC_CLOCK_SOURCE_CPU;
    /* Режим однократного преобразования */
    ADCx_InitStr.ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV;
    ADCx_InitStr.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
    ADCx_InitStr.ADC_ChannelNumber    = ADC_CH_ADC7; /* Номер канала */
    ADCx_InitStr.ADC_Channels         = 0;
    ADCx_InitStr.ADC_LevelControl     = ADC_LEVEL_CONTROL_Disable;
    ADCx_InitStr.ADC_LowLevel         = 0;
    ADCx_InitStr.ADC_HighLevel        = 0;
    ADCx_InitStr.ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL;
    ADCx_InitStr.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_INEXACT;
    ADCx_InitStr.ADC_Prescaler        = ADC_CLK_div_512;
    ADCx_InitStr.ADC_DelayGo          = 7;
    ADC1_Init(&ADCx_InitStr);

    /* Разрешение работы АЦП */
    ADC1_Cmd(ENABLE);

    /* Разрешение работы таймера */
    TIMER_Cmd(MDR_TIMER1, ENABLE);

    /* Настройка приоритета прерывания по Timer1 */
    NVIC_SetPriority(Timer1_IRQn, 13);
    /* Настройка приоритета прерывания по DMA */
    NVIC_SetPriority(DMA_IRQn, 14);

    /* Разрешение прерывания по Timer1 */
    NVIC_EnableIRQ(Timer1_IRQn);
    /* Разрешение прерывания по DMA */
    NVIC_EnableIRQ(DMA_IRQn);

    /* Инициализация системного таймера на период 1 мс */
    init_systick(80000); /* 80 МГц/1кГц = 80000 */

    /* Инициализация драйвера семисегментного индикатора */
    max7219_7seg_init();

    /* Переменная для организации задержки между отображениями напряжения */
    int start = get_systick();

    /* Переменная для хранения усредненного значения кода АЦП */
    uint16_t adc_avg;

    /* Бесконечный цикл */
    for (;;)
    {
        if (adc_buf1_rdy == 1)
        {
            /* Сброс флага готовности буфера */
            adc_buf1_rdy = 0;

            /* Подсчет среднего значения */
            uint32_t acc = 0;
            for (uint32_t i = 0; i < ADC_BUF_SIZE; i++)
                acc += adc_buf1[i];
            acc /= ADC_BUF_SIZE;

            adc_avg = (uint16_t)acc;
        }
        else if (adc_buf2_rdy == 1)
        {
            /* Сброс флага готовности буфера */
            adc_buf2_rdy = 0;

            /* Подсчет среднего значения */
            uint32_t acc = 0;
            for (uint32_t i = 0; i < ADC_BUF_SIZE; i++)
                acc += adc_buf2[i];
            acc /= ADC_BUF_SIZE;

            adc_avg = (uint16_t)acc;
        }

        /* Вызов функций отображения каждые 100 мс */
        if ((get_systick() - start) > 100)
        {
            /* Переводим код АЦП в напряжение и умножаем на 1000 */
            uint32_t voltage = (uint32_t)(((adc_avg / 4095.0f) * (3.3f)) * 1000);

            /* Вывод напряжение на индикатор, установка точки в 4 позиции */
            for (uint32_t i = 1; i <= 4; i++)
            {
                max7219_7seg_write(i, voltage % 10, (i == 4) ? 1 : 0);
                voltage /= 10;
            }
            start = get_systick();
        }
    }
}
