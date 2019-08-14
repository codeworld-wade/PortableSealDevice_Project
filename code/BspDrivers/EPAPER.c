#include "epaper.h"
//#include "picture.h"

#define HAL_SPI_INKSCREEN_MODULE

#ifdef HAL_SPI_INKSCREEN_MODULE
SPI_HandleTypeDef hspi1;
extern const unsigned char gImage_back_white[5000];
extern const unsigned char gImage_num3232[12][64];
#endif

void driver_delay_xms(unsigned long xms)	
{	
  unsigned long i = 0 , j=0;

  for(j=0;j<xms;j++)
  {
    for(i=0; i<256*40; i++);
  }
}

void Epaper_Spi_WriteByte(unsigned char TxData)
{				   			 
  unsigned char TempData;
  unsigned char scnt;
  TempData=TxData;

#ifdef HAL_SPI_INKSCREEN_MODULE
  HAL_SPI_Transmit(&hspi1, &TempData, 1, 1000);	
#else
  EPD_W21_CLK_0;  
  for(scnt=0;scnt<8;scnt++)
  { 
    if(TempData&0x80)
      EPD_W21_MOSI_1 ;
    else
      EPD_W21_MOSI_0 ;
    EPD_W21_CLK_1;  
    EPD_W21_CLK_0;  
    TempData=TempData<<1;
  }
#endif
}

void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
     if(isEPD_W21_BUSY==0) break;;
  }  
}

void Epaper_Write_Command(unsigned char cmd)
{
//  EPD_W21_CS_1;
//  EPD_W21_CS_0;
  EPD_W21_DC_0;  // D/C#   0:command  1:data

  Epaper_Spi_WriteByte(cmd);
//  EPD_W21_CS_1;
}

void Epaper_Write_Data(unsigned char data)
{
//  EPD_W21_CS_1;
//  EPD_W21_CS_0;
  EPD_W21_DC_1;  // D/C#   0:command  1:data

  Epaper_Spi_WriteByte(data);
//  EPD_W21_CS_1;
}
	
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void EPD_HW_Init(void)
{
  EPD_W21_RST_0;     
  driver_delay_xms(100); 
  EPD_W21_RST_1; //hard reset  
  driver_delay_xms(100);
  
  Epaper_READBUSY();   
  Epaper_Write_Command(0x12);  //SWRESET
  Epaper_READBUSY();  
  
  ///<-- 为增加墨水屏显示速度，20190731，片选信号一直打开
  EPD_W21_CS_0;
  driver_delay_xms(100);
          
  Epaper_Write_Command(0x01); //Driver output control      
  Epaper_Write_Data(0xC7);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);

  Epaper_Write_Command(0x11); //data entry mode       
  Epaper_Write_Data(0x01);

  Epaper_Write_Command(0x44); //set Ram-X address start/end position   
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

  Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
  Epaper_Write_Data(0xC7);   //0xC7-->(199+1)=200
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00); 

  Epaper_Write_Command(0x3C); //BorderWavefrom
  Epaper_Write_Data(0x01);	
          
  Epaper_Write_Command(0x18); 
  Epaper_Write_Data(0x80);	

  Epaper_Write_Command(0x22); // //Load Temperature and waveform setting.
  Epaper_Write_Data(0XB1);	
  Epaper_Write_Command(0x20); 


  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(0x00);
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
  Epaper_Write_Data(0xC7);
  Epaper_Write_Data(0x00);
  Epaper_READBUSY();
	
}
/////////////////////////////////////////////////////////////////////////////////////////
void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY();   

}
void EPD_Part_Update(void)
{
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0xFF);   
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY(); 			
}
//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char *datas)
{
   unsigned int i;	
  Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datas, ALLSCREEN_GRAGHBYTES, 3000);
#else 
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(*datas);
			datas++;
   }
#endif
   EPD_Update();	 
}
///////////////////////////Part update//////////////////////////////////////////////
//The x axis is reduced by one byte, and the y axis is reduced by one pixel.
void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
   unsigned int i;   
   const unsigned char  *datas_flag;   
   datas_flag=datas;
	
   Epaper_Write_Command(0x24);   //Write Black and White image to RAM
#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datas, ALLSCREEN_GRAGHBYTES, 3000);
#else 
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(*datas);
			datas++;
   }
#endif
   datas=datas_flag;
   Epaper_Write_Command(0x26);   //Write Black and White image to RAM

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datas, ALLSCREEN_GRAGHBYTES, 3000);
#else 
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(*datas);
			datas++;
   }
#endif
   EPD_Update();		 	 
}
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
	x_start=x_start/8;//转换为字节
	x_end=x_start+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_start;
	if(y_start>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_start+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_start);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    // ????=0	


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_start); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datas, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datas);
			datas++;
   } 
#endif
	 EPD_Part_Update();

}

void EPD_Dis_Part_Not_Updata(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
	x_start=x_start/8;//转换为字节
	x_end=x_start+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_start;
	if(y_start>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_start+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_start);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    // ????=0	


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_start); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datas, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datas);
			datas++;
   } 
#endif
//	 EPD_Part_Update();

}


void EPD_DeepSleep(void)
{  	
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  driver_delay_xms(100);
}

/////////////////////////////////Single display////////////////////////////////////////////////

void EPD_WhiteScreen_White(void)

{
   unsigned int i;//,k;
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)gImage_back_white, ALLSCREEN_GRAGHBYTES, 3000);
#else 
   for(i=0; i<ALLSCREEN_GRAGHBYTES; i++)
   {
      Epaper_Write_Data(0xff);     
   }
#endif  

//   for(k=0;k<200;k++)
//  {
//    for(i=0;i<25;i++)
//    {
//      Epaper_Write_Data(0xff);
//    }
//  }
   Epaper_Write_Command(0x26);   //write RAM for black(0)/white (1)

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)gImage_back_white, ALLSCREEN_GRAGHBYTES, 3000);
#else 
   for(i=0; i<ALLSCREEN_GRAGHBYTES; i++)
   {
      Epaper_Write_Data(0xff);     
   }
#endif     
//  for(k=0;k<200;k++)
//  {
//    for(i=0;i<25;i++)
//    {
//      Epaper_Write_Data(0xff);
//    }
//  }	
  EPD_Update();
}

/////////////////////////////////////TIME///////////////////////////////////////////////////
void EPD_Dis_Part_myself(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
	                       unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
												 unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
												 unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
											   unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,
												 unsigned int PART_COLUMN,unsigned int PART_LINE
	                      )
{
	unsigned int i;  
	unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
	
	//Data A////////////////////////////
	x_startA=x_startA/8;//Convert to byte
	x_end=x_startA+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startA-1;
	if(y_startA>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startA+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startA);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startA); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datasA, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasA);
			datasA++;
   } 
#endif

	//Data B/////////////////////////////////////
	x_startB=x_startB/8;//Convert to byte
	x_end=x_startB+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startB-1;
	if(y_startB>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startB+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startB);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);   


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startB); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datasB, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasB);
			datasB++;
   }
#endif
	 
	//Data C//////////////////////////////////////
	x_startC=x_startC/8;//Convert to byte
	x_end=x_startC+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startC-1;
	if(y_startC>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startC+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startC);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);   


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startC); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datasC, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasC);
			datasC++;
   } 
#endif
	 	 
 	 
	//Data D//////////////////////////////////////
	x_startD=x_startD/8;//Convert to byte
	x_end=x_startD+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startD-1;
	if(y_startD>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startD+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startD);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);        // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startD); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datasD, PART_COLUMN*PART_LINE/8, 3000);
#else 
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasD);
			datasD++;
   } 
#endif	 
	//Data E//////////////////////////////////////
	x_startE=x_startE/8;//Convert to byte
	x_end=x_startE+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startE-1;
	if(y_startE>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startE+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startE);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startE); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM

#ifdef HAL_SPI_INKSCREEN_MODULE
	EPD_W21_DC_1;  // D/C#   0:command  1:data
	HAL_SPI_Transmit(&hspi1, (uint8_t*)datasE, PART_COLUMN*PART_LINE/8, 3000);
#else 
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(*datasE);
			datasE++;
   } 
#endif	
	 EPD_Part_Update();

}

void inkscreen_int(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

   __HAL_RCC_GPIOA_CLK_ENABLE();
//   GPIO_InitStructure.Pin = RES_INKS_Pin | DC_INKS_Pin | SPI1_CS_Pin |\
//                                 SPI1_SCK_Pin | SPI1_MOSI_Pin;    //abcd OE ST LT0输出
  GPIO_InitStructure.Pin = RES_INKS_Pin | DC_INKS_Pin | SPI1_CS_Pin;    //abcd OE ST LT0输出								
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;  
  HAL_GPIO_Init(SPI1_GPIO_Port, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = BUSY_INKS_Pin;    //abcd OE ST LT0输出
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SPI1_GPIO_Port, &GPIO_InitStructure);  

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
//    Error_Handler();
  } 
    __HAL_SPI_ENABLE(&hspi1);   
}

void display_rtc(uint8_t rtc[6])
{
  uint8_t high=0, low=0;
  uint8_t i=0;
  uint8_t temp=0;
  
  /* 1、年的高2位-20 */
  temp = 20;
  high = temp/10;
  low = temp%10;
  i=0;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[high],32,16);
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[low],32,16);
  
  /* 2、年的低2位-xx */
  temp = rtc[0];
  high = temp/10;
  low = temp%10;
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[high],32,16);
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[low],32,16);

  /* 3、- */
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[10],32,16);
  
  /* 4、月的2位 */
  temp = rtc[1];
  high = temp/10;
  low = temp%10;
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[high],32,16);
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[low],32,16);

  /* 5、- */
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[10],32,16);
  
  /* 6、日的2位 */
  temp = rtc[2];
  high = temp/10;
  low = temp%10;
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[high],32,16);      
  i++;
  EPD_Dis_Part_Not_Updata(20+16*i,200,gImage_num3232[low],32,16);

  /* 7、时两位 */
  temp = rtc[3];
  high = temp/10;
  low = temp%10;
  i=0;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[high],32,16);      
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[low],32,16);

  /* 8、： */
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[11],32,16);
  
  /* 9、分两位 */
  temp = rtc[4];
  high = temp/10;
  low = temp%10;
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[high],32,16);      
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[low],32,16);
  
  /* 10、： */
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[11],32,16);
  
  /* 11、秒两位 */
  temp = rtc[5];
  high = temp/10;
  low = temp%10;
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[high],32,16);      
  i++;
  EPD_Dis_Part_Not_Updata(36+16*i,200-64,gImage_num3232[low],32,16);
 
  EPD_Part_Update();
}

//////////////////////////////////////////////////////////////////////////////////////
