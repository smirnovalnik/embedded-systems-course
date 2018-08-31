/**
  ******************************************************************************
  * \file    max7219_7seg.h
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    10.11.2017
  * \brief   Драйвер для MAX7219 с семисегментным индикатором
  ******************************************************************************
*/

/* Макроопределение для предотвращения множественного включения файла */
#ifndef __MAX7219_7SEG_H__
#define __MAX7219_7SEG_H__

/* Макроопределения SPI */
#define MAX7219_SPI                 MDR_SSP2
#define MAX7219_SPI_CLK             RST_CLK_PCLK_SSP2

/* Вывод SPI MOSI
   XP13:11(X26:11)  -> PD6 -> SSP2_TXD */
#define MAX7219_MOSI_PORT_CLK       RST_CLK_PCLK_PORTD
#define MAX7219_MOSI_PORT           MDR_PORTD
#define MAX7219_MOSI_PIN            PORT_Pin_6

/* Вывод SPI SCK
   XP13:10(X26:10)  -> PD5 -> SSP2_CLK */
#define MAX7219_SCK_PORT_CLK        RST_CLK_PCLK_PORTD
#define MAX7219_SCK_PORT            MDR_PORTD
#define MAX7219_SCK_PIN             PORT_Pin_5

/* Вывод SPI NSS
   XP13:24(X26:24)  -> PC0 -> SSP2_FSS */
#define MAX7219_NSS_PORT_CLK        RST_CLK_PCLK_PORTC
#define MAX7219_NSS_PORT            MDR_PORTC
#define MAX7219_NSS_PIN             PORT_Pin_0

/**
  * \brief  Инициализация драйвера семисегментного индикатора
  * \param  Нет
  * \retval Нет
  */
void max7219_7seg_init(void);

/**
  * \brief  Вывод цифры на семисегментный индикатор
  * \param  pos - номер знакоместа (1 - 8), dig - цифра для отображения (0 - 9),
  *         dot - наличие десятичной точки
  * \retval Нет
  */
void max7219_7seg_write(char pos, char dig, char dot);

#endif /* __MAX7219_7SEG_H__ */
