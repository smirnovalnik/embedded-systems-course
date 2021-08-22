/**
  ******************************************************************************
  * \file    uart_io.h
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.08.2017
  * \brief   Инициализация UART и перенаправление стандартного потока
  *          ввода/вывода на UART.
  ******************************************************************************
*/

/* Макроопределение для предотвращения множественного включения файла */
#ifndef __UART_IO_H__
#define __UART_IO_H__

/* Скорость обмена по UART, бод */
#define UART_IO_BAUDRATE        115200

/* Макрооределения UART для отладочной платы К1986ВЕ92QI(MDR32F9Q2I) */
#define UART_IO                 MDR_UART2
#define UART_IO_CLK             RST_CLK_PCLK_UART2

/* Вывод UART TX */
#define UART_IO_TX_PORT_CLK     RST_CLK_PCLK_PORTF
#define UART_IO_TX_PORT         MDR_PORTF
#define UART_IO_TX_PIN          PORT_Pin_1

/* Вывод UART RX */
#define UART_IO_RX_PORT_CLK     RST_CLK_PCLK_PORTF
#define UART_IO_RX_PORT         MDR_PORTF
#define UART_IO_RX_PIN          PORT_Pin_0

/**
  * \brief  Инициализация интерфейса UART для стандартного потока ввода/вывода.
  * \note   Функция должна вызываться после настройки тактирования
  *         микроконтроллера и перед вызовом функций для работы
  *         со стандартным потоком ввода/вывода.
  * \param  Нет
  * \retval Нет
  */
void uart_io_init(void);

#endif /* __UART_IO_H__ */
