#include "infraredpin.h"


void infrared_detect_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : KEY_Pin */
  GPIO_InitStruct.Pin = PE_DECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PE_DECT_GPIO_Port, &GPIO_InitStruct);
}

uint8_t infrared_pin_read(void)
{
    uint8_t retVal=0;
    if(HAL_GPIO_ReadPin(PE_DECT_GPIO_Port, PE_DECT_Pin) == GPIO_PIN_SET)
        retVal = 1;
    return retVal;
}