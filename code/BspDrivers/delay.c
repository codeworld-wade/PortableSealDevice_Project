/**
 * timer driver file 
 * timer6: time base
 * timer7: time base interrupt
 */

#include "delay.h"

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

static uint8_t timFlag_7 = 0;

void time_init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 0;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
//      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
//      Error_Handler();
    }

    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 0;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
//        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
//        Error_Handler();
    }  
}

void delay_ms(uint32_t ms)
{
#ifdef TIMER_INTERRUPT_ENABLE   
    htim7.Init.Prescaler = (uint32_t)(36000 - 1);    // 2khz frequency
    htim7.Init.Period = (uint32_t)(2*ms - 1);
    TIM_Base_SetConfig(htim7.Instance, &(htim7.Init));
    
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE); 
//    htim7.Instance->SR = 0;
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
    {
        /* Starting Error */
//        Error_Handler();
    }
    while(timFlag_7 != 1);
    HAL_TIM_Base_Stop_IT(&htim7);
    timFlag_7 = 0;
#else
    htim6.Init.Prescaler = (uint32_t)(48000 - 1);
    htim6.Init.Period = (uint32_t)(ms - 1);
    TIM_Base_SetConfig(htim6.Instance, &(htim6.Init));
    
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_IT_UPDATE);       
    
    HAL_TIM_Base_Start(&htim6);
    while((__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE)) != SET);
    HAL_TIM_Base_Stop(&htim6);
    
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_IT_UPDATE);
#endif
}

void delay_us(uint32_t us)
{
#ifdef TIMER_INTERRUPT_ENABLE
    htim7.Init.Prescaler = (uint32_t)(36 - 1);    // 2Mhz frequency
    htim7.Init.Period = (uint32_t)(2*us - 1);
    TIM_Base_SetConfig(htim7.Instance, &(htim7.Init));
    
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);     
  /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
    {
        /* Starting Error */
//        Error_Handler();
    }
    while(timFlag_7 != 1);
    HAL_TIM_Base_Stop_IT(&htim7);
    timFlag_7 = 0;    
#else  
    htim6.Init.Prescaler = (uint32_t)(48 - 1);
    htim6.Init.Period = (uint32_t)(us - 1);
    TIM_Base_SetConfig(htim6.Instance, &(htim6.Init));
    
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_IT_UPDATE);       
    
    HAL_TIM_Base_Start(&htim6);
    while((__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE)) != SET);
    HAL_TIM_Base_Stop(&htim6);
    
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_IT_UPDATE);       
#endif
}

void TIM7_IRQHandler(void) 
{
  HAL_TIM_IRQHandler(&htim7);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM7)
        timFlag_7 = 1;
}