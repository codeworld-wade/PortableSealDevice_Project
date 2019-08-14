/*
 * timer driver file 
 * timer1: pwm output
 * timer6: time base
 * timer7: time base interrupt
 */

#ifndef __INFRAREDPIN_H
#define __INFRAREDPIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

//#define TIMER_INTERRUPT_ENABLE
#define PE_DECT_Pin GPIO_PIN_3
#define PE_DECT_GPIO_Port GPIOC

extern void infrared_detect_init(void);
extern uint8_t infrared_pin_read(void);

#ifdef __cplusplus
}
#endif

#endif
