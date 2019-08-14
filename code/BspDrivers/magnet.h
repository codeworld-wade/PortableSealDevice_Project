/*
 *  key driver file
 */

#ifndef __MAGNET_H
#define __MAGNET_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
   
#define MAGNET_Pin GPIO_PIN_6
#define MAGNET_GPIO_Port GPIOC

#define MAGNET_OPEN 1
#define MAGNET_CLOSE 0

#define ELE_TIM_PERIOD  (100)   // 电磁铁周期
#define ELE_TIM_PRESCALER (48)  // 电磁铁分频

#define ELE_COMPARE_FULL (100)
#define ELE_COMPARE_HALF (50)   

extern void magnet_init(void);
extern void magnet_control(uint8_t status);


#ifdef __cplusplus
}
#endif


#endif
