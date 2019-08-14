/********************************************************************************
  * @file    ../BspDrivers/hy40r204.h 
  * @author  wei wei
  * @version V0.1
  * @date    07/01/2019
  * @brief   This file contains the headers of the hy40r204 handlers.
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
#ifndef __HY40R204_H
#define __HY40R204_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "bsp_conf.h"

/* Exported macro ------------------------------------------------------------*/
#define BLE_UART_BAUD_RATE 256000
#define BLE_UART_REC_LEN 100
   
#define HY40R204_WAKE_OK    0
#define HY40R204_WAKE_NOT_OK 1

#define HY40R204_RESET_OK   0
#define HY40R204_RESET_NOT_OK   1
   
/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t bleCmdRecFlag;  // 蓝牙模块完整接收-命令应答数据标识
    uint8_t bleOtherRecFlag;// 蓝牙模块完整接收-非命令数据标识
    uint8_t len;            // 蓝牙模块有效数据长度
    uint8_t interFlag;      // 蓝牙模块发出数据标识
    uint8_t *pBuf; // 蓝牙有效数据缓冲
} hy40r204_rec_data_s;

extern hy40r204_rec_data_s hy40r204RecData;
extern uint8_t uart1DataBuf[BLE_UART_REC_LEN];
/* Exported constants --------------------------------------------------------*/

/* Exported variable ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void hy40r204_para_init(void);
extern void hy40r204_init(void);
extern void hy40r204_send_data(uint8_t *buf, uint16_t len);
extern void hy40r204_wake_con(uint8_t state);
extern void hy40r204_reset_con(uint8_t state);
//void delaynms(uint16_t time);


#ifdef __cplusplus
}
#endif

#endif /* __INKSCREEN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
