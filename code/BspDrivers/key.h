/*
 *  key driver file
 */

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
   
#define KEY_Pin GPIO_PIN_5
#define KEY_GPIO_Port GPIOC

extern void key_init(void);
extern uint8_t key_input_check(void);
extern uint8_t key_input_filter_check(void);


#ifdef __cplusplus
}
#endif


#endif
