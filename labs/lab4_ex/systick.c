/**
  ******************************************************************************
  * \file    systick.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    15.11.2017
  * \brief   Функции для формирования временных задержек с помощью SysTick
  ******************************************************************************
*/

#include "systick.h"
#include <MDR32Fx.h>

volatile uint32_t systick_ms;

void init_systick(uint32_t ticks)
{
    systick_ms = 0;
    SysTick_Config(ticks);
    NVIC_SetPriority(SysTick_IRQn, 15);
    NVIC_EnableIRQ(SysTick_IRQn);
}

uint32_t get_systick(void)
{
    return systick_ms;
}

void delay(uint32_t ms)
{
    uint32_t start = 0;

    start = get_systick();
    while((get_systick() - start) < ms);
}

void interrupt_systick(void)
{
    systick_ms++;
}
