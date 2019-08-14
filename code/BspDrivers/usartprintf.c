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
/* Includes ------------------------------------------------------------------*/
#include "usartprintf.h"

/* Exported macro ------------------------------------------------------------*/
#define USART3_HAL_REC_LEN 1
/* Exported types ------------------------------------------------------------*/
/* Exported variable ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
uint16_t usart3RxSta = 0;       
uint8_t usart3RxBuf[USART3_REC_LEN];
uint8_t usart3HalRxBuf[USART3_HAL_REC_LEN];
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void usart_printf_init(void)
{
    /* 1��Initialize  usart3 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = USART3_BAUDRATE;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
//        Error_Handler();
    }
    /* 2��Enable usart3, rx interrupt and disable UART_TXE*/
    HAL_UART_Receive_IT(&huart3, usart3HalRxBuf, USART3_HAL_REC_LEN);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������  
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
    {
        usart3RxSta |= (3<<14);
        if(usart3RxSta&0x3FFF>=USART3_REC_LEN)    usart3RxSta = 0;
        else usart3RxSta++;
        usart3RxBuf[usart3RxSta&0x3FFF]=usart3HalRxBuf[0];
    }
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
	uint32_t timeout=0;
	HAL_UART_IRQHandler(&huart3);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>3000) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&huart3, usart3HalRxBuf, USART3_HAL_REC_LEN) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>3000) break;	
	}
}

void usart3_tx(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    HAL_UART_Transmit(&huart3, pData, Size, Timeout);
    while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)!=SET);		//�ȴ����ͽ���    
}

uint16_t usart3_get_flag(void)
{
    return usart3RxSta;
}

void usart3_get_data(uint8_t *pData, uint16_t Size)
{
    uint16_t tempVal = 0;
    if((!Size)||(pData == NULL))
        return;
    tempVal = usart3RxSta&0x3FFF;
    tempVal = (Size > tempVal)?tempVal:Size;
    memcpy(pData, usart3RxBuf, tempVal);    
}

/*******************************************************************************
	��������fputc
	��  ��:
	��  ��:
	����˵����
	�ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
  USART3->DR = (uint8_t) ch;      
  return ch;
}

/*******************************************************************************
	��������fputc
	��  ��:
	��  ��:
	����˵����
	�ض���getc��������������ʹ��scanf�����Ӵ���1��������
*******************************************************************************/
int fgetc(FILE *f)
{
    while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) == 0);
	return (int)((__HAL_UART_FLUSH_DRREGISTER(&huart3))&((uint16_t)0x01FF));
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

