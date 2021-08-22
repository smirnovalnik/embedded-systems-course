/**
  ******************************************************************************
  * \file    max7219_7seg.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    10.11.2017
  * \brief   Драйвер для MAX7219 с семисегментным индикатором
  *****************************************************************************/

/* Подключение заголовочного файла с макроопределениями
   регистров специальных функций К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32Fx.h>
/* Подключение заголовочного файла с конфигурацией
   Standard Peripheral Library (SPL) для К1986ВЕ92QI(MDR32F9Q2I) */
#include <MDR32F9Qx_config.h>
/* Подключение функций управления портами */
#include <MDR32F9Qx_port.h>
/* Подключение функций управления интерфейсом
   Synchronous Serial Port - SSP (SPI) */
#include <MDR32F9Qx_ssp.h>
/* Подключение функций управления тактированием */
#include <MDR32F9Qx_rst_clk.h>

/* Подключение заголовочного файла, содержащего платформо-зависимые
   макроопределения */
#include "max7219_7seg.h"

/* Структура для инициализации SSP */
SSP_InitTypeDef             SSP_InitStr;

void max7219_7seg_init(void)
{
    /* Разрешение тактирования периферийных модулей */
    RST_CLK_PCLKcmd(MAX7219_SPI_CLK |
                    MAX7219_MOSI_PORT_CLK |
                    MAX7219_SCK_PORT_CLK |
                    MAX7219_NSS_PORT_CLK , ENABLE);

    PORT_InitTypeDef            PORT_InitStructure;

    /* Настройка выходов SPI */
    /* Подтяжка к питанию отключена */
    PORT_InitStructure.PORT_PULL_UP     = PORT_PULL_UP_OFF;
    /* Подтяжка к нулю отключена */
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    /* Настройка ножки порта на вывод */
    PORT_InitStructure.PORT_OE          = PORT_OE_OUT;
    /* Задание скорости нарастания на выводе порта */
    PORT_InitStructure.PORT_SPEED       = PORT_SPEED_FAST;
    /* Ножка порта в цифровом режиме */
    PORT_InitStructure.PORT_MODE        = PORT_MODE_DIGITAL;
    /* Альтернативная функция порта */
    PORT_InitStructure.PORT_FUNC        = PORT_FUNC_ALTER;

    /* Указываем номер ножки, которую будет инициализировать */
    PORT_InitStructure.PORT_Pin         = MAX7219_MOSI_PIN;
    /* Инициализация ножки */
    PORT_Init(MAX7219_MOSI_PORT, &PORT_InitStructure);

    /* Указываем номер ножки, которую будет инициализировать */
    PORT_InitStructure.PORT_Pin         = MAX7219_SCK_PIN;
    /* Инициализация ножки */
    PORT_Init(MAX7219_SCK_PORT, &PORT_InitStructure);

    /* Вторая альтернативная функция */
    PORT_InitStructure.PORT_FUNC        = PORT_FUNC_OVERRID;
    /* Указываем номер ножки, которую будет инициализировать */
    PORT_InitStructure.PORT_Pin         = MAX7219_NSS_PIN;
    /* Инициализация ножки */
    PORT_Init(MAX7219_NSS_PORT, &PORT_InitStructure);

    /* Настройка SPI */
    /* Сброс SPI модуля */
    SSP_DeInit(MAX7219_SPI);

    /* Разрешение тактирования SPI */
    SSP_BRGInit(MAX7219_SPI, SSP_HCLKdiv16);

    /* Заполнение полей структуры SSP_InitStr значениями по умолчанию */
    SSP_StructInit(&SSP_InitStr);

    /* Частота fsck = 20 кГц */
    SSP_InitStr.SSP_SCR  = 0x80;
    SSP_InitStr.SSP_CPSDVSR = 2;
    /* Режим - мастер */
    SSP_InitStr.SSP_Mode = SSP_ModeMaster;
    /* Размер слова для передачи - 16 */
    SSP_InitStr.SSP_WordLength = SSP_WordLength16b;
    /* Настройка фронта и полярности сигнала SCK */
    SSP_InitStr.SSP_SPH = SSP_SPH_1Edge;
    SSP_InitStr.SSP_SPO = SSP_SPO_Low;
    /* Режим - SPI */
    SSP_InitStr.SSP_FRF = SSP_FRF_SPI_Motorola;
    /* Аппаратное управление NSS */
    SSP_InitStr.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
    /* Инициализация SPI */
    SSP_Init(MAX7219_SPI, &SSP_InitStr);

    /* Разрешение работы SPI */
    SSP_Cmd(MAX7219_SPI, ENABLE);

    /* Инициализация MAX7219 */

    /* Выключение тестового режима MAX7219 */
    /* Ожидание флага "Пустой буфер" */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    /* Передача данных */
    SSP_SendData(MAX7219_SPI, 0x0F00);

    /* Включение MAX7219 */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MAX7219_SPI, 0x0C01);

    /* Режим дешифрации */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MAX7219_SPI, 0x09FF);

    /* Отображение 4 символов */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MAX7219_SPI, 0x0B03);

    /* Настройка яркости: 5/32 */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MAX7219_SPI, 0x0A02);
}

void max7219_7seg_write(char pos, char dig, char dot)
{
    /* Ожидание флага "Пустой буфер" */
    while (SSP_GetFlagStatus(MAX7219_SPI, SSP_FLAG_TFE) == RESET);
    /* Передача данных */
    SSP_SendData(MAX7219_SPI, (pos << 8) | (dot << 7) | dig);
}
