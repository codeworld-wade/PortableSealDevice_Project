#include "magnet.h"
#include "delay.h"

TIM_HandleTypeDef htim3;
void magnet_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_AFIO_REMAP_TIM3_ENABLE();

    /*Configure GPIO pins : KEY_Pin */
    GPIO_InitStruct.Pin = MAGNET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MAGNET_GPIO_Port, &GPIO_InitStruct);

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = ELE_TIM_PRESCALER - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = ELE_TIM_PERIOD - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
    //    Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
    //    Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
    //    Error_Handler();
    } 
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

/*******************************************************************************
* Function Name  : magnet_control.
* Description    : µÁ¥≈Ã˙¥¶¿Ì.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void magnet_control(uint8_t status)
{
    if(status == MAGNET_OPEN)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ELE_COMPARE_FULL);
        delay_ms(500);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ELE_COMPARE_HALF);
    }
    else if(status == MAGNET_CLOSE)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    }	
}