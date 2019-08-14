/*
 *  key driver file
 */

#include "key.h"

/* local function */
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pins : KEY_Pin */
    GPIO_InitStruct.Pin = KEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);
}

uint8_t key_input_check(void)
{
    uint8_t retVal = 0;
    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
        retVal = 1;
    else 
        retVal = 0;
    return retVal;
}

uint8_t key_input_filter_check(void)
{
    uint8_t retVal = 0;
    static uint8_t cnt = 0;
    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
    {
      cnt++;
      if(cnt > 15)
      {
        cnt = 0;
        retVal = 1;
      }
    }
    else
        cnt = 0;

    return retVal;
}
