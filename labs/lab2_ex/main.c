/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.1
  * \date    1.08.2017
  * \brief   Пример проекта для выполнения лабораторной работы №2.
  *          Программа измеряет минимальную ширину импульса сигнала,
  *          поданного на вход X3 (COMP_IN). Для измерения используется таймер
  *          в режиме входного захвата. Источником сигнала является генератор
  *          импульсов произвольной формы. Шаг измерения ширины - 1 мкс.
  *          Диапазон измерения: 0 - 65535 мкс  Измеренная ширина выводится
  *          на эмулятор терминала на персональном компьютере командой pulse.
  *          Сброс измерений выполняется командой reset.
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

/* Настройка тактового сигнала центрального процессора (ЦП) микроконтроллера.
   После запуска микроконтроллер начинает тактироваться от внутреннего
   высокочастотного RC генератора (HSI, 8 МГц). Стабильности частоты данного
   генератора не достаточно для согласования скоростей передачи UART
   микроконтроллера и COM порта персонального компьютера.
   На отладочной плате установлен внешний кварцевый резонатор (HSE, 8 МГц),
   который обладает достаточной стабильностью для согласования UART и COM порта.
   Функция sys_clk_init включает умножитель частоты (ФАПЧ) с опорным сигналом
   HSE. Суммарный коэффициент умножения задается коэффициентом предделителя HSE,
   коэффициентом умножения ФАПЧ и делителем тактового сигнала ЦП.
   Частота ЦП: fhse * (1/div_hse) * pll_mul * (1/cpu_div) =
   8 МГц * (1/1) * 10 * (1/1) = 80 МГц
   Для более потробной информации смотри схему формирования тактовой частоты в
   спецификации К1986ВЕ92QI(MDR32F9Q2I).
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

/*----------------------Ваше решение начинается здесь-------------------------*/

/*  Разъем X3 (COMP_IN) подключен к ножке 2 порта E. Данная ножка в режиме
    альтернативной функции подключается к каналу входного захвата 3 таймера 2
    (TMR2_CH3). Канал входного захвата имеет два регистра захвата. Первый (CCR)
    настраивается на фронт импульса, второй (CCR1) - на спад. Прерывание
    вызывается по спаду (захвату CCR1). В обработчике прерывания
    (Timer2_IRQHandler) длительность импульса вычисляется как разница CCR1 - CCR.
          ____         ____
    _____|    |_______|    |_____
         ^     ^
         CCR   CCR1 -> Timer2_IRQHandler
    Результат вычисления сохраняется в переменной min_pulse.
    Вывод результата осуществляется в бесконечном цикле функции main при вводе
    строки содержащей слово "period".
*/

/* Переменная для сохранения минимальной ширины импульса */
volatile uint16_t min_pulse = 0xFFFF;

/* Обработчик прерывания по Timer2 */
void Timer2_IRQHandler(void)
{
    uint16_t pulse, pos, neg;

    /* Вычисление ширины импульса */
    pos = TIMER_GetChnCapture(MDR_TIMER2, TIMER_CHANNEL3);
    neg = TIMER_GetChnCapture1(MDR_TIMER2, TIMER_CHANNEL3);

    if (neg > pos)
    {
        /*  0xFFFF
                           _/|
                         _/  |
                       _/    |
                     _/      |
            0x0000 _/________|
                      _____
                   __|     |__
                     ^     ^
                     pos   neg
        */
        pulse = neg - pos;
    }
    else
    {
        /*  0xFFFF
                           _/|        _/|
                         _/  |      _/  |
                       _/    |    _/    |
                     _/      |  _/      |
            0x0000 _/________|_/________|
                           _____
                        __|     |__
                          ^     ^
                          pos   neg
        */
        pulse = 0xFFFF - (pos - neg);
    }

    /* Определение минимальной ширины импульса */
    if (pulse < min_pulse)
        min_pulse = pulse;

    /* Сброс флага прерывания по входному захвату (по спаду, регистр CCR1) */
    TIMER_ClearITPendingBit(MDR_TIMER2, TIMER_STATUS_CCR_CAP1_CH3);
}
/*----------------------Ваше решение заканчивается здесь----------------------*/

/* Функция main. Точка входа в программу */
int main(void)
{
    /* Вызов функции настройки частоты тактирования ЦП */
    sys_clk_init();

    /* Вызов функции инициализации UART для стандартного потока */
    uart_io_init();

/*----------------------Ваше решение начинается здесь-------------------------*/
    /* Разрешение тактирования периферийных модулей */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTE | RST_CLK_PCLK_TIMER2, ENABLE);

    /* Структура для инициализации порта */
    PORT_InitTypeDef Port_InitStructure;

    /* Заполнение полей структуры Port_InitStructure значениями по умолчанию */
    PORT_StructInit(&Port_InitStructure);

    /* Заполнение полей структуры Port_InitStructure */
    /* Подтяжка к питанию отключена */
    Port_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
    /* Подтяжка к нулю отключена */
    Port_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    /* Триггер Шмитта отключен */
    Port_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF;
    /* Режим работы выхода – управляемый драйвер */
    Port_InitStructure.PORT_PD = PORT_PD_DRIVER;
    /* Режим работы входного фильтра - фильтр выключен */
    Port_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
    /* Альтернативная функция порта ->
       канал входного захвата 3 таймера 2 (TMR2_CH3) */
    Port_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
    /* Скорость нарастания/спада - максимально быстрый фронт */
    Port_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    /* Режим работы - цифровой вход/выход */
    Port_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    /* Настройка ножки порта на ввод */
    Port_InitStructure.PORT_OE = PORT_OE_IN;
    /* Указываем номер ножки,которую будет инициализировать */
    Port_InitStructure.PORT_Pin = PORT_Pin_2;
    /* Инициализация ножки */
    PORT_Init(MDR_PORTE, &Port_InitStructure);

    /* Параметры Timer2
       TIMER2CLK = 80 MHz, Prescaler = 80, TIMER2 counter clock = 1 MHz */
    /* Разрешение тактирования Timer2 */
    TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv1);

    /* Структура для инициализации таймера */
    TIMER_CntInitTypeDef TIM_CntInit;

    /* Заполнение полей структуры TIM_CntInit значениями по умолчанию */
    TIMER_CntStructInit(&TIM_CntInit);

    /* Заполнение полей структуры TIM_CntInit */
    /* Установка предделителя */
    TIM_CntInit.TIMER_Prescaler                = 80;
    /* Установка периода счета */
    TIM_CntInit.TIMER_Period                   = 0xFFFF;
    /* Направление счета не меняется */
    TIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
    /* Направление счета - "вверх" */
    TIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;

    /* Инициализация Timer2 */
    TIMER_CntInit(MDR_TIMER2, &TIM_CntInit);

    /* Структура для инициализации канала входного захвата таймера */
    TIMER_ChnInitTypeDef TIM_ChnInit;

    /* Заполнение полей структуры TIM_ChnInit значениями по умолчанию */
    TIMER_ChnStructInit(&TIM_ChnInit);

    /* Заполнение полей структуры TIM_ChnInit */
    /* Используем третий канал входного захвата/сравнения */
    TIM_ChnInit.TIMER_CH_Number              = TIMER_CHANNEL3;
    /* Режим входного захвата */
    TIM_ChnInit.TIMER_CH_Mode                = TIMER_CH_MODE_CAPTURE;
    /* Входной захват в регистре CCR по фронту */
    TIM_ChnInit.TIMER_CH_EventSource         = TIMER_CH_EvSrc_PE;
    /* Разрешение работы регистра CCR1*/
    TIM_ChnInit.TIMER_CH_CCR1_Ena = ENABLE;
    /* Входной захват в регистре CCR1 по спаду */
    TIM_ChnInit.TIMER_CH_CCR1_EventSource    = TIMER_CH_CCR1EvSrc_NE;
    /* Немедленное обновление регистров CCR и CCR1 */
    TIM_ChnInit.TIMER_CH_CCR_UpdateMode      = TIMER_CH_CCR_Update_Immediately;

    /* Инициализация канала входного захвата 3 */
    TIMER_ChnInit(MDR_TIMER2, &TIM_ChnInit);

    /* Разрешение прерывания по захвату отрицательного фронта (CCR1) */
    TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CCR_CAP1_CH3, ENABLE);

    /* Разрешение работы Timer2 */
    TIMER_Cmd(MDR_TIMER2, ENABLE);

    /* Настройка приоритета прерывания по Timer2 */
    NVIC_SetPriority(Timer2_IRQn, 15);
    /* Разрешение прерывания по Timer2 */
    NVIC_EnableIRQ(Timer2_IRQn);

    /* Печать строки приветствия в стандартный поток */
    printf("Measuring minimum pulse width\n");

    /* Буфер для команды */
    char cmd[10 + 1];

    /* Бесконечный цикл */
    for (;;)
    {
        /* Ожидание ввода команды */
        scanf("%10s", cmd);

        /* Сравнение введенной команды */
        if (strcmp(cmd, "pulse") == 0)
        {
            /* Печать строки в стандартный поток */
            printf("%d us\n", min_pulse);
        }
        else if (strcmp(cmd, "reset") == 0)
        {
            min_pulse = 0xFFFF;
        }
        else
        {
            printf("Unknown command\n");
        }
    }
/*----------------------Ваше решение заканчивается здесь----------------------*/
}
