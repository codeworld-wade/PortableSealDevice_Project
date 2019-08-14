/*
 * timer driver file 
 * timer1: pwm output
 * timer6: time base
 * timer7: time base interrupt
 */

#ifndef __TIME_H
#define __TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

//#define TIMER_INTERRUPT_ENABLE

extern void time_init(void);
extern void delay_ms(uint32_t ms);
extern void delay_us(uint32_t us); 

#ifdef __cplusplus
}
#endif

#endif
