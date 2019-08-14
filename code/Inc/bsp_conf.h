/********************************************************************************
  * @file    ../BspDrivers/bsp_config.h 
  * @author  wei wei
  * @version V0.1
  * @date    07/01/2019
  * @brief   This file contains the headers of the inkscreen handlers.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_CONF_H
#define __BSP_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "a4988.h"
#include "delay.h"
#include "usartprintf.h" 
#include "hy40r204.h"
#include "epaper.h"
#include "key.h"
#include "infraredpin.h"
#include "rtc.h"
#include "magnet.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/  
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __INKSCREEN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
