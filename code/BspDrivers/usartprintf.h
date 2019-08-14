/********************************************************************************
  * @file    ../BspDrivers/usartprint.h 
  * @author  wei wei
  * @version V0.1
  * @date    07/01/2019
  * @brief   This file contains the headers of the ble handlers.
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
#ifndef __USARTPRINTF_H
#define __USARTPRINTF_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "string.h"
/* Exported macro ------------------------------------------------------------*/
#define USART3_BAUDRATE 115200  // 串口3波特率
#define USART3_REC_LEN  64      //串口3最大接收字节数   
/* Exported types ------------------------------------------------------------*/
/* Exported variable ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void usart_printf_init(void);
extern void usart3_tx(uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern uint16_t usart3_get_flag(void);
extern void usart3_get_data(uint8_t *pData, uint16_t Size);

#ifdef __cplusplus
}
#endif

#endif /* __USARTPRINTF_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
