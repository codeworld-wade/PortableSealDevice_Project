#include "hy40r204.h"
#include "string.h"

#define INT_BLE_Pin GPIO_PIN_7
#define NRST_BLE_Pin GPIO_PIN_8
#define WK_BLE_Pin GPIO_PIN_9
#define BLE_GPIO_Port GPIOC

UART_HandleTypeDef huart1;
hy40r204_rec_data_s hy40r204RecData;
uint8_t uart1DataBuf[BLE_UART_REC_LEN] = {0};

//static void hy40r204_para_init(void);

void hy40r204_para_init(void)
{
    memset(uart1DataBuf, 0, sizeof(uart1DataBuf));
    hy40r204RecData.bleCmdRecFlag = 0;
    hy40r204RecData.bleOtherRecFlag = 0;
    hy40r204RecData.interFlag = 0;
    hy40r204RecData.len = 0;
    hy40r204RecData.pBuf = uart1DataBuf;
}

void hy40r204_init(void)
{
    /* 1、Initialize INT_PIN,NRST_PIN,WK_PIN */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : INT_BLE_Pin */
    GPIO_InitStruct.Pin = INT_BLE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BLE_GPIO_Port, &GPIO_InitStruct);
      /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    GPIO_InitStruct.Pin = NRST_BLE_Pin|WK_BLE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BLE_GPIO_Port, &GPIO_InitStruct);
    
    /* 2、Initialize USART1 for hy40r204 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = BLE_UART_BAUD_RATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
//        Error_Handler();
    }

    /* 3、Enable usart1, rx interrupt and disable UART_TXE*/
    HAL_GPIO_WritePin(BLE_GPIO_Port, NRST_BLE_Pin, GPIO_PIN_SET);
    __HAL_UART_ENABLE(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);

    /* 4、Initialize hy40r204 recData param */
    hy40r204_para_init();
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
}

void hy40r204_wake_con(uint8_t state)
{
    HAL_GPIO_WritePin(BLE_GPIO_Port, WK_BLE_Pin, (GPIO_PinState)state);   
}

void hy40r204_reset_con(uint8_t state)
{    
    hy40r204_para_init();
    HAL_GPIO_WritePin(BLE_GPIO_Port, NRST_BLE_Pin, (GPIO_PinState)state); 
}

void hy40r204_send_data(uint8_t *buf, uint16_t len)
{      
//    unsigned char i;
//    for(i=0; i<len; i++){
        if(HAL_UART_Transmit(&huart1, buf, len, 1000)!= HAL_OK)
        {}
//            break;
//        }
}

void USART1_IRQHandler(void)
{  
  if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))&&\
      (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE)))
  {
      __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);            
      hy40r204RecData.pBuf[hy40r204RecData.len] = USART1->DR;
      hy40r204RecData.len++;
      hy40r204RecData.interFlag = 1;
      if(hy40r204RecData.len+1>=99)
          hy40r204RecData.len=0;   
  }
}

void EXTI9_5_IRQHandler(void)
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
    hy40r204_wake_con(HY40R204_WAKE_OK);
  }    
}