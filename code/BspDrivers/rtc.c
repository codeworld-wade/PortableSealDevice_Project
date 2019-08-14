#include "rtc.h"

RTC_HandleTypeDef hrtc;

static void RTC_CalendarConfig(void);

void rtc_init(void)
{
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
//    Error_Handler();
  }

  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
  {
    RTC_CalendarConfig();
  }
  else
  {
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }  
}

static void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x19;
  sdatestructure.Month = RTC_MONTH_JULY;
  sdatestructure.Date = 0x29;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
 //   Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = 0x17;
  stimestructure.Minutes = 0x25;
  stimestructure.Seconds = 0x00;

  if (HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
//    Error_Handler();
  }

  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}

void RTC_CalendarShow(uint8_t buf[6])
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  buf[0] = sdatestructureget.Year;
  buf[1] = sdatestructureget.Month;
  buf[2] = sdatestructureget.Date;

  buf[3] = stimestructureget.Hours;
  buf[4] = stimestructureget.Minutes;
  buf[5] = stimestructureget.Seconds;
}