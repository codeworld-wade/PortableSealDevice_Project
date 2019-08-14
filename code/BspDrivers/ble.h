/********************************************************************************
  * @file    ../BspDrivers/ble.h 
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
#ifndef __BLE_H
#define __BLE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "bsp_conf.h"

/* Exported macro ------------------------------------------------------------*/
#define BLE_MODULE_INFO_ENABLE 1

#if BLE_MODULE_INFO_ENABLE
#define BLE_WORK_IDLE 0
#define BLE_WORK_BUSY 1
#define BLE_ROLE_SLAVE 0
#define BLE_ROLE_HOST 1
#define BLE_STATUS_DISCONNECT 0
#define BLE_STATUS_CONNECT 1
   
// Length of bd addr as a string
#define B_ADDR_STR_LEN     15
//! Default Public and Random Address Length
#define B_ADDR_LEN    6
   
/* Exported types ------------------------------------------------------------*/
typedef struct{
    uint8_t conStatus;  // 蓝牙连接状态
    uint8_t role;       // 蓝牙模块当前角色，主机-1；从机-0
    uint8_t workStatus; // 蓝牙模块的工作状态，0-IDLE, 1-BUSY
    uint8_t rsv;
    uint8_t connectSlaveAddr[8];    // 蓝牙做为主机，连接的从机MAC地址
    uint8_t selfSlaveAddr[8];       // 蓝牙作为从机，自己本身的MAC地址
} ble_status_info_s;

/* Exported variable ---------------------------------------------------------*/
extern ble_status_info_s bleStaInfo;
#endif
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void str_to_hex(unsigned char *str,unsigned char *buf);
//extern unsigned char *ble_addr_to_str( unsigned char *pAddr );
extern unsigned char ble_addr_to_str( unsigned char *pAddr, unsigned char pAddrString[B_ADDR_STR_LEN] );
extern void ble_data_mode(void);   

extern unsigned char ble_slave_mac_show(void);
extern unsigned char ble_test(void);

extern unsigned char ble_slave_init(void);
extern unsigned char ble_host_init(void);

extern unsigned char ble_connection_check(unsigned char host_slave_flag);

extern unsigned char ble_host_scanf_slave(void);
extern unsigned char ble_host_connect_slave(unsigned char connt,unsigned char slavenumb,unsigned char *mac);

extern unsigned char ble_data_send(unsigned char *buf,unsigned char len);
extern unsigned char ble_data_receive(unsigned char *buf, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif /* __INKSCREEN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
