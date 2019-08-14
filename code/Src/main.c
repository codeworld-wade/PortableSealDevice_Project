/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "picture.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLE_CONNECT_FLAG        1
#define BLE_DISCONNECT_FLAG     0
    
/* Private macro -------------------------------------------------------------*/
///<-- 2�Ż�
#define MOTOR_STEP_CNT_INTF_ORIGIN   (76*16) // �ӵ�һ�κ��ⶨλ�����Ĳ�����20190812��76->80
#define MOTOR_STEP_CNT_INTF_MAX      (991*16)// �ӵ�һ�κ��ⶨλ�����µĲ�����20190812
#define MOTOR_STEP_CNT_INTF_INKPAD   (416*16)// �ӵ�һ�κ��ⶨλ��ӡ��Ĳ�����20190812
//#define MOTOR_STEP_CNT_INTF_MAX      (990*16)// �ӵ�һ�κ��ⶨλ�����µĲ�����20190812
///<-- 1�Ż�
//#define MOTOR_STEP_CNT_INTF_ORIGIN   (110*16) // �ӵ�һ�κ��ⶨλ�����Ĳ�����20190812
//#define MOTOR_STEP_CNT_INTF_MAX      (1100*16)// �ӵ�һ�κ��ⶨλ�����µĲ�����20190812
//#define MOTOR_STEP_CNT_INTF_INKPAD   (410*16)// �ӵ�һ�κ��ⶨλ��ӡ��Ĳ�����20190812

//#define MOTOR_STEP_CNT_MAX      (MOTOR_STEP_CNT_INTF_ORIGIN+MOTOR_STEP_CNT_INTF_MAX)
#define MOTOR_STEP_CNT_INKPAD   (MOTOR_STEP_CNT_INTF_ORIGIN+MOTOR_STEP_CNT_INTF_INKPAD)
#define MOTOR_STEP_CNT_MAX      (MOTOR_STEP_CNT_INTF_MAX)

//#define MOTOR_STEP_CNT_MAX      (998*16)
//#define MOTOR_STEP_CNT_INKPAD   (423*16)

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);

/* Private user code ---------------------------------------------------------*/     
typedef enum {
    BLE_CMD_MIN,            // ����ָ��-��Сֵ
    BLE_CMD_MOTOR_BEGIN,    // ����ָ��-��ʼ
    BLE_CMD_MOTOR_STOP,     // ����ָ��-ֹͣ
    BLE_CMD_MOTOR_STAMP,    // ����ָ��-����
    BLE_CMD_MOTOR_UP,       // ����ָ��-̧��
    BLE_CMD_MOTOR_DOWN,     // ����ָ��-�·�
    BLE_CMD_PICTURE_SW,     // ����ָ��-ͼƬ�л�
    BLE_CMD_RESET,          // ����ָ��-��λ��ԭ��
    BLE_CMD_UNLOCK,         // ����ָ��-������20190812
    BLE_CMD_LOCK,           // ����ָ��-������20190812
    BLE_CMD_MODULE_NAME,    // ����ָ��-�޸�����ģ�����֣�20190813
    BLE_CMD_MAX             // ����ָ��-���ֵ
} remote_ble_cmd_e;    
 
typedef struct {
  remote_ble_cmd_e cmd;     // Զ���յ���ָ��
  uint8_t para;
} remote_rec_cmd_s;   
 
char defaultName[] = "AT+NAME=BLE#0A434F1416687\r\n";
char moduleName[] = "AT+NAME=HL02\r\n";

void step_motor_back_origin(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
 int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
    stepper_init();
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
  /* Configure the system clock */
    SystemClock_Config();
//    MX_RTC_Init();        
  /* Initialize all configured peripherals */
//    stepper_init();
    time_init();
    usart_printf_init();
    hy40r204_init();
    inkscreen_int();
    EPD_HW_Init();
    key_init();
    infrared_detect_init();
    magnet_init();
    rtc_init();
    
  // 1����������
   remote_rec_cmd_s bleCmd;   
   uint32_t stepCnt=0;
   uint8_t bleConStatusNow=0;   
   uint8_t bleConStatusLast=0;
   uint8_t name = 0;
  // 2��������Բ���
//   uint32_t tim = 0;
   uint8_t len = 0;
   uint8_t buf[100];
//   stepper_dir_e dir=STEPPER_DIR_FORWARD;
   stepper_microstep_e mode = FULL_STEP_MODE;
//   uint16_t tim = 0;
  
  // 3�����ڲ���
  uint8_t cnt=0;
  /* Infinite loop */
/* �̶�����15968 = 16x998, ������Ĳ���6768 = 423x16*/   
   
//  tim = 1000;
  mode = SIXTEENTH_STEP_MODE;
//  uint8_t picNum=0;
  
  // 4��Һ����Ļ��������
//  unsigned char fen_L,fen_H,miao_L,miao_H;
//  uint8_t hex=0;  
  // 5����������
  uint8_t keyCnt = 0;
  uint8_t systemStatus = 0;
  
    /* 6�����Ժ���ԭʼλ�ýڵ� */
//  uint8_t infrared_retVal = 0;
//  stepper_sleep_control(STEPPER_SLEEP_MODE);
    /* 7��RTCʱ�Ӷ�ȡ�������� */
    uint8_t rtcBuf[6] = {0};
//    uint8_t rtcMinTemp = 0;
  ///<-- ʹ�õ��ʱ��������򿪣��Ӷ����͹��ģ�20190730
  stepper_sleep_control(STEPPER_SLEEP_MODE);
  /* 8��������Ӧ����ʱ���� */
//  uint8_t i=0;
  uint8_t cntTim = 0;   ///<-- ������ʱ������20190814
  uint8_t inkScreenStartFlag = 0;
  while (1)
  {
    
    // 1�����ָʾ�Ʋ���
//   delay_ms(tim);
//   stepper_dir_led_control(STEPPER_DIR_FORWARD, MOTOR_LED_ON);
//   stepper_dir_led_control(STEPPER_DIR_REVERSE, MOTOR_LED_ON);
//   delay_ms(tim);
//   stepper_dir_led_control(STEPPER_DIR_FORWARD, MOTOR_LED_OFF);
//   stepper_dir_led_control(STEPPER_DIR_REVERSE, MOTOR_LED_OFF);
    // 2�����������֤
//     dir = STEPPER_DIR_FORWARD;
//     mode = SIXTEENTH_STEP_MODE;
//     stepCnt = 0;
//     tim = 1000;
//     stepper_sleep_control(STEPPER_WAKEUP_MODE);
//     while(1)
//     {
//       dir = dir;
//       stepCnt = stepCnt;
//  //    mode = mode;
//       tim = tim;
//       stepper_dir_control(dir);
//       stepper_microstep_control(mode);
//       stepper_set_level_control(STEPPER_LEVEL_LOW);
//       delay_us(tim);      
//       stepper_set_level_control(STEPPER_LEVEL_HIGH);
//       delay_us(tim);
//       stepCnt++;
//       printf("stepCnt:%d\r\n", stepCnt);
//       
//       if(infrared_pin_read() != systemStatus)
//       {
//         systemStatus = infrared_pin_read();
//       }
//     }  
    // 3������Һ�������ַ���ʾ
//    EPD_HW_Init(); 
//    EPD_WhiteScreen_White();
//   
//    EPD_Dis_Part(32,32,gImage_font_icon,32,32);
//    delay_ms(5000);
//    EPD_DeepSleep(); 
    /* 1���������м�飬�͹�����ƣ�δ���ӣ���Ҫ�ۺϿ��Ƕ�ʱ���жϵ���Դ��ʱ����� */
    while(!systemStatus)
    {
      if(key_input_check()==1)
        keyCnt++;
      else
        keyCnt = 0;
      delay_ms(100);
      
      if( keyCnt> 30)
      {
        keyCnt = 0;
        systemStatus = 1;
        /* 1��Һ����ʼ״̬��־ - 20190812*/
        inkScreenStartFlag = 0;
        /* 2������״̬���������־ - 20190812 */
        hy40r204_para_init();        
      }
    }
    
    /* 2��ϵͳ���п���ҳ������ */
    if(systemStatus == 1)
    {
      EPD_HW_Init();
      EPD_WhiteScreen_ALL(gImage_huilang_icon);
      delay_ms(2000);
      systemStatus  = 2;
     
//      for(i=0; i<2; i++)
//      {
//        EPD_Dis_Part(0,200,gImage_back_black,200,200);
//        EPD_Dis_Part(0,200,gImage_back_white,200,200);        
//      }
      EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
      EPD_Part_Update();
      EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
      EPD_Part_Update(); 
      
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);     
    }    
    // 3�����ڵ���ʵ��
    // 4������ģ�����
    if(systemStatus != 2)
      continue;
    /* 2��ϵͳ�ػ�ʱ�Ķ��� */
    if(key_input_check()==1)
      keyCnt++;
    else
      keyCnt = 0;
    if(keyCnt > 3)
    {
      keyCnt = 0;
      systemStatus = 0;
      // �����Ļ����
      EPD_HW_Init(); //Electronic paper initialization
      EPD_WhiteScreen_White(); //Show all white   
      inkScreenStartFlag = 1;
    }
    
     delay_ms(500);
     if((inkScreenStartFlag == 0)&&(systemStatus == 2))
     {        
        RTC_CalendarShow(rtcBuf); 
        display_rtc(rtcBuf);
     }
     
     stepper_microstep_control(mode);  
     if(hy40r204RecData.interFlag)
     {  
        len = hy40r204RecData.len;
        memcpy(buf, hy40r204RecData.pBuf, BLE_UART_REC_LEN);
        hy40r204RecData.len = 0;
        hy40r204RecData.interFlag = 0;

        if(strncmp((char *)buf, "AT+CON=SUCCESS", 14) == 0)
          bleConStatusNow = BLE_CONNECT_FLAG;
        else if(strncmp((char *)buf, "AT+CON=STOP", 11) == 0)
          bleConStatusNow = BLE_DISCONNECT_FLAG;
        
        // 1��ָ���ȡ��ʱ���շ������������ִ�У����֧����϶�����Ҫ��������
        if(strncmp((char *)buf, "stamp", 5)==0)
        {
            bleCmd.cmd = BLE_CMD_MOTOR_STAMP;
        }
        else if(strncmp((char *)buf, "up", 2) == 0)
        {
          bleCmd.cmd = BLE_CMD_MOTOR_UP;
        }
        else if(strncmp((char*)buf, "down", 4) == 0)
        {
          bleCmd.cmd = BLE_CMD_MOTOR_DOWN;
        }
        else if(strncmp((char*)buf, "picture", 7)==0)
        {
          bleCmd.cmd = BLE_CMD_PICTURE_SW;
        }
        else if(strncmp((char*)buf, "reset", 5)==0)
        {
          bleCmd.cmd = BLE_CMD_RESET;
        }
        else if(strncmp((char*)buf, "unlock", 6)==0)
        {
          bleCmd.cmd = BLE_CMD_UNLOCK;
        }
        else if(strncmp((char*)buf, "lock", 4)==0)
        {
          bleCmd.cmd = BLE_CMD_LOCK; 
        }
        else if(strncmp((char*)buf, "name", 4)==0)
        {
          bleCmd.cmd = BLE_CMD_MODULE_NAME;
        }
        
        // 5.1 ����������ɺ󣬽��ճɹ��ַ�
        if(bleConStatusNow == BLE_CONNECT_FLAG)
        {
//          hy40r204_send_data(buf, len);
          if((bleCmd.cmd > BLE_CMD_MIN)&&(bleCmd.cmd < BLE_CMD_MAX))
            hy40r204_send_data("OK", 2);           
        }
             
        while(len--)  printf("%c",buf[cnt++]);
        cnt = 0;
        memset(buf, 0, sizeof(buf));
     }               
         
    if(bleConStatusNow == BLE_CONNECT_FLAG)
    {
      switch(bleCmd.cmd)
      {       
        case BLE_CMD_MOTOR_STAMP: 
        { 
          // 0.1�����Ӹ�������ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();          
          EPD_Dis_Part_Not_Updata(52,200-100,gImage_font_icon[0],32,32);
          EPD_Dis_Part_Not_Updata(52+32,200-100,gImage_font_icon[1],32,32);
          EPD_Dis_Part_Not_Updata(52+32*2,200-100,gImage_font_icon[2],32,32);
          EPD_Part_Update();

          inkScreenStartFlag = 1;
          magnet_control(MAGNET_OPEN);
          delay_ms(3000);
                    
          stepper_sleep_control(STEPPER_WAKEUP_MODE);
          
          if(stepCnt == MOTOR_STEP_CNT_MAX)
          {
            stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_MAX+2000);
            delay_ms(2000);
            stepper_walk(STEPPER_DIR_REVERSE, 2000); 
            step_motor_back_origin();          
          }
          else
          {           
            stepper_walk(STEPPER_DIR_FORWARD, 2000);
            delay_ms(2000);
            stepper_walk(STEPPER_DIR_REVERSE, 2000);             
            step_motor_back_origin();              
          }
          stepCnt = 0;
          
          stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_MAX);
          stepCnt = MOTOR_STEP_CNT_MAX;
          
          // 2��ͣ��8s->4sʱ��
          delay_ms(4000);
          stepper_walk(STEPPER_DIR_FORWARD, stepCnt);
          step_motor_back_origin();
          stepCnt = 0;
          magnet_control(MAGNET_CLOSE);         
          stepper_sleep_control(STEPPER_SLEEP_MODE);

          // 1�����ݵ�ǰλ��ȷ�����²�����STEPPER_DIR_REVERSE-��ʾ���£���������
//           stepper_sleep_control(STEPPER_WAKEUP_MODE);
          
//           stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_MAX-stepCnt); 
//           stepCnt = MOTOR_STEP_CNT_MAX;
          
//           magnet_control(MAGNET_CLOSE);
//           // 2��ͣ��8s->4sʱ��
//           delay_ms(4000);
          
//           // 3���ָ���մӡ���λ�ã�STEPPER_DIR_FORWARD-��ʾ���ϣ�������С
//           stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_MAX-MOTOR_STEP_CNT_INKPAD);
// //          stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_INTF_MAX);
//           stepCnt = MOTOR_STEP_CNT_MAX-MOTOR_STEP_CNT_INKPAD;
//           stepper_sleep_control(STEPPER_SLEEP_MODE);
        }
        break;
        case BLE_CMD_MOTOR_UP:
        {
          // 0.1�����������ƶ�����ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();                     
          EPD_Dis_Part_Not_Updata(20,200-100,gImage_font_icon[3],32,32);
          EPD_Dis_Part_Not_Updata(20+32,200-100,gImage_font_icon[4],32,32);
          EPD_Dis_Part_Not_Updata(20+32*2,200-100,gImage_font_icon[6],32,32);
          EPD_Dis_Part_Not_Updata(20+32*3,200-100,gImage_font_icon[7],32,32);
          EPD_Dis_Part_Not_Updata(20+32*4,200-100,gImage_font_icon[2],32,32);
          EPD_Part_Update();
          
          inkScreenStartFlag = 1;
          magnet_control(MAGNET_OPEN);
          delay_ms(3000);          
          stepper_sleep_control(STEPPER_WAKEUP_MODE);
          // 1�����ϻָ���ԭ��λ�ã�ͨ���������λ�ö�λ
//          if(stepCnt > 0)
//          {
//            stepper_walk(STEPPER_DIR_FORWARD, stepCnt); 
//            step_motor_back_origin();
//            stepCnt = 0;             
//          }
          if(stepCnt == MOTOR_STEP_CNT_MAX)
          {
            stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_MAX);
            step_motor_back_origin();
            stepCnt = 0;             
          }
          magnet_control(MAGNET_CLOSE);
          delay_ms(2000);
          stepper_sleep_control(STEPPER_SLEEP_MODE);
        }
        break;
        case BLE_CMD_MOTOR_DOWN:
        {
          // 0.1�����������ƶ�����ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();                    
          EPD_Dis_Part_Not_Updata(20,200-100,gImage_font_icon[3],32,32);
          EPD_Dis_Part_Not_Updata(20+32,200-100,gImage_font_icon[5],32,32);
          EPD_Dis_Part_Not_Updata(20+32*2,200-100,gImage_font_icon[6],32,32);
          EPD_Dis_Part_Not_Updata(20+32*3,200-100,gImage_font_icon[7],32,32);
          EPD_Dis_Part_Not_Updata(20+32*4,200-100,gImage_font_icon[2],32,32);
          EPD_Part_Update();
          inkScreenStartFlag = 1;
          // 2������֪�������ֵ��ͨ����������λ�ö�λ
          magnet_control(MAGNET_OPEN);
          delay_ms(3000);          
          stepper_sleep_control(STEPPER_WAKEUP_MODE);
          
//          if(stepCnt <= MOTOR_STEP_CNT_MAX)
//          {
//            step_motor_back_origin();
//            stepCnt = 0;
//            stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_MAX); 
//            stepCnt = MOTOR_STEP_CNT_MAX;              
//          }
          if(stepCnt == 0)
          {
            step_motor_back_origin();
            stepCnt = 0;
            stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_MAX); 
            stepCnt = MOTOR_STEP_CNT_MAX;             
          }
          magnet_control(MAGNET_CLOSE);
          delay_ms(2000);
          stepper_sleep_control(STEPPER_SLEEP_MODE);
        }
        break;
        
        case BLE_CMD_PICTURE_SW: 
        {
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update(); 
          EPD_Dis_Part(0,200,gImage_qr_code,200,200);
          inkScreenStartFlag = 1; 
          
          while(cntTim < 10)
          {
            delay_ms(1000);
            if(hy40r204RecData.interFlag)
            {
              len = hy40r204RecData.len;
              memcpy(buf, hy40r204RecData.pBuf, BLE_UART_REC_LEN);
              hy40r204RecData.len = 0;
              hy40r204RecData.interFlag = 0;
              if(strncmp((char *)buf, "picture", 7) == 0)
              {
                hy40r204_send_data("OK", 2);
                cntTim = 10;
              }
              //  �ַ����ԵĹ���  
              while(len--)  printf("%c",buf[cnt++]);
              cnt = 0;
              memset(buf, 0, sizeof(buf));
              // ��ʱ�����л�����
              cntTim++;
             }
          } 
          cntTim = 0;  
        }
        break;
        
        case BLE_CMD_RESET:
        {
          // 0.1������У׼��ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();                   
          EPD_Dis_Part_Not_Updata(68,200-100,gImage_font_icon[10],32,32);
          EPD_Dis_Part_Not_Updata(68+32,200-100,gImage_font_icon[11],32,32);
          EPD_Part_Update();
          magnet_control(MAGNET_OPEN);
          delay_ms(3000);          
          inkScreenStartFlag = 1;
          stepper_sleep_control(STEPPER_WAKEUP_MODE);
//          stepCnt = 0;
          if(stepCnt == MOTOR_STEP_CNT_MAX)
          {
            stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_MAX);
            step_motor_back_origin();
            stepCnt = 0;
          }
          else if(stepCnt == 0)
          {
            step_motor_back_origin();
            stepCnt = 0;           
          }
         
          
          // 1������ȥ4000��
//          stepper_walk(STEPPER_DIR_REVERSE, 4000);              
//          
//          delay_ms(500);
//          // �����Ժ㶨�ٶ����ᣬ������һ��ͻ��ʱ���ͼ�ס����
//          while(infrared_pin_read() == 0)
//          {
//            // 1�����û���źţ�������
//
//             dir = STEPPER_DIR_FORWARD;
//             tim = 1000;
//             mode = SIXTEENTH_STEP_MODE;
//             
//             stepper_dir_control(dir);
//             stepper_microstep_control(mode);
//             stepper_set_level_control(STEPPER_LEVEL_LOW);
//             delay_us(tim);      
//             stepper_set_level_control(STEPPER_LEVEL_HIGH);
//             delay_us(tim);
//             stepCnt++;
//          }
//          
//          stepCnt = MOTOR_STEP_CNT_INTF_ORIGIN;
//          while(stepCnt--)
//          {
//            // 1�����û���źţ�������
//             dir = STEPPER_DIR_FORWARD;
//             tim = 1000;
//             mode = SIXTEENTH_STEP_MODE;
//             
//             stepper_dir_control(dir);
//             stepper_microstep_control(mode);
//             stepper_set_level_control(STEPPER_LEVEL_LOW);
//             delay_us(tim);      
//             stepper_set_level_control(STEPPER_LEVEL_HIGH);
//             delay_us(tim);
//          }
          magnet_control(MAGNET_CLOSE);
          stepCnt = 0;
          delay_ms(2000);
          stepper_sleep_control(STEPPER_SLEEP_MODE);
        }
        break;
        
        case BLE_CMD_UNLOCK:
        {
          // 0.1�����ӿ�����ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();                   
          EPD_Dis_Part_Not_Updata(68,200-100,gImage_font_icon[9],32,32);
          EPD_Dis_Part_Not_Updata(68+32,200-100,gImage_font_icon[8],32,32);
          EPD_Part_Update();
          inkScreenStartFlag = 1;
          magnet_control(MAGNET_OPEN);
          delay_ms(3000);
        }
        break;
        
        case BLE_CMD_LOCK:
        {
          // 0.1������������ʾ
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
          EPD_Part_Update();
          EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
          EPD_Part_Update();          
          EPD_Dis_Part_Not_Updata(68,200-100,gImage_font_icon[4],32,32);
          EPD_Dis_Part_Not_Updata(68+32,200-100,gImage_font_icon[8],32,32);
          EPD_Part_Update();
          inkScreenStartFlag = 1;
          magnet_control(MAGNET_CLOSE);
          delay_ms(3000);
        }
        break;

        case BLE_CMD_MODULE_NAME:
        {
          if(++name>1) name =0;
          
          if(!name)
           hy40r204_send_data((uint8_t *)defaultName, 27);
          else 
           hy40r204_send_data((uint8_t *)moduleName, 14);
          
//          while(hy40r204RecData.interFlag)
//          {  
//            len = hy40r204RecData.len;
//            memcpy(buf, hy40r204RecData.pBuf, BLE_UART_REC_LEN);
//            hy40r204RecData.len = 0;
//            hy40r204RecData.interFlag = 0;
////            if(strncmp((char *)buf, "AT+OK", 5) == 0)
//            while(len--)  printf("%c",buf[cnt++]);
//            cnt = 0;
//            memset(buf, 0, sizeof(buf));
//          }
        }
        break;
        
        default: break;
      }
     
     bleCmd.cmd = BLE_CMD_MIN;
     if(inkScreenStartFlag == 1)
     {
       EPD_Dis_Part_Not_Updata(0,200,gImage_back_white,200, 200);
       EPD_Part_Update();
       EPD_Dis_Part_Not_Updata(0,200,gImage_back_black,200, 200);
       EPD_Part_Update();
       
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);
      inkScreenStartFlag = 0;
     }        
    }
   
    if((bleConStatusNow == BLE_DISCONNECT_FLAG)&&(bleConStatusLast == BLE_CONNECT_FLAG))
    {
      bleCmd.cmd  = BLE_CMD_MIN; 
      RTC_CalendarShow(rtcBuf);
      
      EPD_Dis_Part(0,200,gImage_back_black,200,200);
      EPD_Dis_Part(0,200,gImage_back_white,200,200);    
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);
      EPD_Dis_Part(0,200,gImage_back_icon,200,200);
      inkScreenStartFlag = 0;      
    }
    
    if(bleConStatusLast != bleConStatusNow)
      bleConStatusLast = bleConStatusNow;
    
    /* 5��Һ����Ļ����ͨ�� */
//   /* 6�����Ժ���ԭʼλ�ýڵ� */
//    infrared_retVal = infrared_pin_read();
//    magnet_control(MAGNET_OPEN);
//    delay_ms(5000);
//    magnet_control(MAGNET_CLOSE);
//    delay_ms(5000);
//    /* 7��rtcʱ����غ��� */
//    RTC_CalendarShow(rtcBuf);
//    delay_ms(5000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
//  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
//  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
  
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }  
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

void step_motor_back_origin(void)
{
   stepper_dir_e dir=STEPPER_DIR_FORWARD;
   stepper_microstep_e mode = FULL_STEP_MODE;
   uint16_t tim = 0;
   uint8_t retVal = 0;
   
  tim = 1000;
  mode = SIXTEENTH_STEP_MODE;
  stepper_microstep_control(mode);
  retVal = infrared_pin_read();
  if(retVal == 0)
  {
    dir = STEPPER_DIR_FORWARD;
    stepper_dir_control(dir);    
    while(infrared_pin_read() == 0)
    {
      stepper_set_level_control(STEPPER_LEVEL_LOW);
      delay_us(tim);      
      stepper_set_level_control(STEPPER_LEVEL_HIGH);
      delay_us(tim);
    }    
  }
  else if(retVal == 1)
  {
    dir = STEPPER_DIR_REVERSE;
    stepper_dir_control(dir);
    while(infrared_pin_read() == 1)
    {
      stepper_set_level_control(STEPPER_LEVEL_LOW);
      delay_us(tim);      
      stepper_set_level_control(STEPPER_LEVEL_HIGH);
      delay_us(tim);
    }    
  }

  
//  // 1.1 У�鲽������һ��������У�鲽��
//  #define MOTOR_STEP_CNT_CHECK_MAIN (250*16)
//  #define MOTOR_STEP_CNT_CHECK_SUB (125*16)
//  #define MOTOR_STEP_CNT_INTF_MAX_CHECK (MOTOR_STEP_CNT_INTF_MAX-MOTOR_STEP_CNT_CHECK_SUB) 
//  while(retVal != 1)
//  {
//    if(infrared_pin_read() == 1)
//    {
//      stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_CHECK_MAIN);
//      if(infrared_pin_read() == 1)
//      {
//        stepper_walk(STEPPER_DIR_REVERSE, MOTOR_STEP_CNT_CHECK_SUB);
//      }
//      else 
//      {
//        dir = STEPPER_DIR_FORWARD;
//        tim = 1000;
//        mode = SIXTEENTH_STEP_MODE;
//        stepper_dir_control(dir);
//        stepper_microstep_control(mode);             
//        while(infrared_pin_read() == 0)
//        {
//          stepper_set_level_control(STEPPER_LEVEL_LOW);
//          delay_us(tim);      
//          stepper_set_level_control(STEPPER_LEVEL_HIGH);
//          delay_us(tim);
//        }
//        retVal = 1;
//      }  
//    }
//    else if(infrared_pin_read() == 0)
//    {
//      stepper_walk(STEPPER_DIR_FORWARD, MOTOR_STEP_CNT_CHECK_MAIN);
//      continue;
//    }
//  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
