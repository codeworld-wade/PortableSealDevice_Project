/********************************************************************************
  * @file    ../BspDrivers/inkscreen.h 
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
#ifndef __INKSCREEN_H
#define __INKSCREEN_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define xDot 200
#define yDot 200
#define DELAYTIME 1500
/* Exported functions ------------------------------------------------------- */

extern void driver_delay_xms(unsigned long xms);
extern void inkscreen_int(void);

extern void inkscreen_full_init(void);
/**********************************************************
������ Label  =1 ��ʾ disBuffer�е�����
	            =0 ��ʾ disBuffer�е�һ���ֽڵ�����
***********************************************************/
extern void inkscreen_dis_full((unsigned char *)disBuffer, unsigned char label);

extern void inkscreen_part_init(void);
/**********************************************************
������ 
		xStart			x����ʼ����
		xEnd			  x�����������
		yStart			y����ʼ����
		yEnd			  y�����������
		disBuffer		��ʾbuffer
		label      	=1 ��ʾ disBuffer�е�����
	   		        =0 ��ʾ disBuffer�е�һ���ֽڵ�����
***********************************************************/
extern void inkscreen_dis_part(unsigned char xStart, unsigned char xEnd, unsigned long yStart, \
                                unsigned long yEnd, unsigned char *disBuffer, unsigned char label);
//POWER OFF
extern void EPD_W21_POWEROFF(void); //POWER OFF
//SLEEP MODE
extern void EPD_W21_SLEEP(void);//SLEEP MODE


#ifdef __cplusplus
}
#endif

#endif /* __INKSCREEN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
