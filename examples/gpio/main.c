/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    27.10.2023
  * \brief   Пример проекта с использованием портов ввода/вывода.
  *          Программа осуществляет мигание светодиодом, подключенным к PC0.
    *          При нажатии на кнопку UP (PB5) светодиод горит постоянно.
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

int main()
{
    /* Разрешение тактирования периферийных модулей */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB |
                    RST_CLK_PCLK_PORTC |
                    RST_CLK_PCLK_PORTE, ENABLE);

    /* Настройка портов ввода/вывода (GPIO) */
    PORT_InitTypeDef Port_InitStructure;
    /* Заполнение полей структуры PORT_InitStructure значениями по умолчанию */
    PORT_StructInit(&Port_InitStructure);

    /* Указываем номер ножки, которую будет инициализировать */
    Port_InitStructure.PORT_Pin = PORT_Pin_0;
    /* Настройка ножки порта на вывод */
    Port_InitStructure.PORT_OE = PORT_OE_OUT;
    /* Функции ножки - порт ввода/вывода */
    Port_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    /* Скорость нарастания фронта минимальная */
    Port_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
    /* Цифровой режим ножки порта */
    Port_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    /* Инициализация ножки */
    PORT_Init(MDR_PORTC, &Port_InitStructure);

    /* Указываем номер ножки, которую будет инициализировать */
    Port_InitStructure.PORT_Pin = PORT_Pin_5;
    /* Подтяжка к питанию отключена */
    Port_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
    /* Подтяжка к земле отключена */
    Port_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    /* Настройка ножки порта на ввод */
    Port_InitStructure.PORT_OE = PORT_OE_IN;
    /* Функции ножки - порт ввода/вывода */
    Port_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    /* Цифровой режим ножки порта */
    Port_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    /* Инициализация ножки */
    PORT_Init(MDR_PORTB, &Port_InitStructure);

    for(;;)
    {
        int btn = PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_5);

        if (btn != 0)
        {
            for (int i = 0; i < 100000; i++);
            PORT_SetBits(MDR_PORTC, PORT_Pin_0);
            for (int i = 0; i < 100000; i++);
            PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
        }
        else
        {
            PORT_SetBits(MDR_PORTC, PORT_Pin_0);
        }
    }
}
