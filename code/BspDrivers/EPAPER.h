/*
 * timer driver file 
 * timer1: pwm output
 * timer6: time base
 * timer7: time base interrupt
 */

#ifndef __EPAPER_H
#define __EPAPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

//250*122///////////////////////////////////////
#define MONOMSB_MODE 1
#define MONOLSB_MODE 2 
#define RED_MODE     3

#define MAX_LINE_BYTES 25// =200/8
#define MAX_COLUMN_BYTES  200

#define ALLSCREEN_GRAGHBYTES	5000

///////////////////////////GPIO Settings//////////////////////////////////////////////////////
#define BUSY_INKS_Pin GPIO_PIN_1
#define RES_INKS_Pin GPIO_PIN_2
#define DC_INKS_Pin GPIO_PIN_3
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_GPIO_Port GPIOA   
  
#define EPD_W21_MOSI_0	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_RESET)
#define EPD_W21_MOSI_1	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_SET)

#define EPD_W21_CLK_0	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_SCK_Pin, GPIO_PIN_RESET)
#define EPD_W21_CLK_1	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_SCK_Pin, GPIO_PIN_SET)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(SPI1_GPIO_Port, DC_INKS_Pin, GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(SPI1_GPIO_Port, DC_INKS_Pin, GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(SPI1_GPIO_Port, RES_INKS_Pin, GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(SPI1_GPIO_Port, RES_INKS_Pin, GPIO_PIN_SET)

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY HAL_GPIO_ReadPin(SPI1_GPIO_Port, BUSY_INKS_Pin) 


extern void inkscreen_int(void);  
extern void driver_delay_xms(unsigned long xms);			
extern void Epaper_READBUSY(void);
extern void Epaper_Spi_WriteByte(unsigned char TxData);
extern void Epaper_Write_Command(unsigned char cmd);
extern void Epaper_Write_Data(unsigned char data);

extern void EPD_HW_Init(void); //Electronic paper initialization
extern void EPD_Update(void);

extern void EPD_Part_Init(void);//Local refresh initialization
extern void EPD_Part_Update(void); 

extern void EPD_WhiteScreen_White(void);
extern void EPD_DeepSleep(void);
//Display 
extern void EPD_WhiteScreen_ALL(const unsigned char *datas);
extern void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
extern void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,\
                   unsigned int PART_COLUMN,unsigned int PART_LINE);
extern void EPD_Dis_Part_myself(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,\
	                  unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,\
                          unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,\
                          unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,\
                          unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,\
                          unsigned int PART_COLUMN,unsigned int PART_LINE);
extern void EPD_Dis_Part_Not_Updata(unsigned int x_start,unsigned int y_start,const unsigned char * datas,\
                          unsigned int PART_COLUMN,unsigned int PART_LINE);
extern void display_rtc(uint8_t rtc[6]);
  
  

#ifdef __cplusplus
}
#endif

#endif




