/*
 *  a4988 stepper driver file
 */
#include "a4988.h"
#include "delay.h"

/* private macro */  
#define MS3_MOTOR_Pin GPIO_PIN_10
#define MS2_MOTOR_Pin GPIO_PIN_11
#define MS1_MOTOR_Pin GPIO_PIN_12
#define MS_MOTOR_GPIO_PORT GPIOC

#define SLEEP_N_MOTOR_Pin GPIO_PIN_2
#define SLEEP_N_MOTOR_GPIO_Port GPIOD

#define CW_MOTOR_Pin GPIO_PIN_1
#define CCW_MOTOR_Pin GPIO_PIN_2
#define RST_N_MOTOR_Pin GPIO_PIN_3
#define DIR_MOTOR_Pin GPIO_PIN_4
#define STEP_MOTOR_Pin GPIO_PIN_5
#define STEP_MOTOR_GPIO_Port GPIOB

//#define CCW_MCU_GPIO_Port GPIOB

//#define ACC_STEP_NUM 50
unsigned short AccStep[ACC_STEP_NUM] = {
1818,1805,1792,1779,1767,1761,1748,1736,1724,1712,
1701,1689,1678,1667,1656,1645,1634,1623,1613,1603,
1592,1582,1572,1562,1548,1538,1529,1520,1506,1497,
1488,1475,1466,1458,1445,1437,1425,1416,1404,1397,
1385,1377,1366,1355,1348,1337,1326,1316,1309,1299,
1289,1279,1269,1259,1250,1241,1232,1222,1214,1205,
1196,1188,1179,1168,1160,1152,1144,1134,1126,1116,
1109,1101,1092,1085,1075,1066,1059,1050,1042,1033,
1027,1018,1010,1002,994,986,978,971,963,956,
949,940,933,926,917,911,904,896,890,882,
874,868,861,853,847,840,833,826,820,813,
806,800,794,786,780,774,767,761,754,749,
742,736,730,724,717,712,706,700,694,689,
682,677,671,666,660,654,649,644,638,632,
627,622,617,611,606,601,596,591,585,581,
576,571,566,561,556,551,547,542,538,532,
528,524,519,514,510,506,502,497,493,488,
484,480,476,472,468,463,460,455,452,448,
444,440,436,432,428,425,421,417,414,410,
407,403,399,396,392,389,386,382,379,375,
372,369,366,363,359,356,353,350,347,344,
341,338,335,332,329,326,323,320,317,315,
312,309,307,304,301,299,296,293,291,288,
286,283,281,278,276,273,271,268,266,264,
261,259,257,255,252,250,248,246,244,242,
239,237,235,233,231,229,227,225,223,221,
219,217,215,214,212,210,208,206,204,203,
201,199,197,196,194,192,191,189,187,186,
184,183,181,179,178,176,175,173,172,170,
169,167,166,165,163,162,160,159,158,156,
155,154,152,151,150,149,147,146,145,144,
142,141,140,139,138,137,135,134,133,132,
131,130,129,128,127,126,125,124,123,122,
121,120,119,118,117,116,115,114,113,112,
111,110,109,108,107,107,106,105,104,103,
102,102,101,100,99,98,98,97,96,95,
95,94,93,92,92,91,90,90,89,88,
87,87,86,85,85,84,84,83,82,82,
81,80,80,79,79,78,77,77,76,76,
75,75,74,74,73,73,72,71,71,70,
70,69,69,68,68,67,67,67,66,66,
65,65,64,64,63,63,62,62,62,61,
61,60,60,60,59,59,58,58,58,57,
57,56,56,56,55,55,55,54,54,54,
53,53,53,52,52,52,51,51,51,50,
50,50,49,49,49,49,48,48,48,47,
47,47,47,46,46,46,45,45,45,45,
44,44,44,44,43,43,43,43,42,42,
42,42,42,41,41,41,41,40,40,40,
40,40,39,39,39,39,39,38,38,38,
38,38,37,37,37,37,37,37,36,36,
36,36,36,36,35,35,35,35,35,35,
34,34,34,34,34,34,34,33,33,33,
33,33,33,33,32,32,32,32,32,32,
32,32,31,31,31,31,31,31,31,31,
30,30,30,30,30,30,30,30,30,29,
29,29,29,29,29,29,29,29,29,28,
28,28,28,28,28,28,28,28,28,28,
27,27,27,27,27,27,27,27,27,27,
27,27,27,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,25,25,25,
25,25,25,25,25,25,25,25,25,25,
25,25,25,25,24,24,24,24,24,24,
24,24,24,24,24,24,24,24,24,24,
24,24,24,24,23,23,23,23,23,23,
23,23,23,23,23,23,23,23,23,23,
23,23,23,23,23,23,23,23,23,22,
22,22,22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,22,22,
22,22,22,21,21,21,21,21,21,21,
21,21,21,21,21,21,21,21,21,21,
21,21,21,21,21,21,21,21,21,21,
21,21,21,21,21,21,21,21,21,21,
21,21,21,21,21,21,21,21,21,21,
21,21,21,21,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,20,20,
20,20,20,20,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,19,
};
   
/* private function */
static void stepper_gpio_hw_init(void);
static void stepper_delay_us(uint32_t us);

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void stepper_gpio_hw_init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, MS3_MOTOR_Pin|MS2_MOTOR_Pin|MS1_MOTOR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, SLEEP_N_MOTOR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RST_N_MOTOR_Pin|DIR_MOTOR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, CW_MOTOR_Pin|CCW_MOTOR_Pin, GPIO_PIN_SET);
    
    /*Configure GPIO pins : MS3_MOTOR_Pin MS2_MOTOR_Pin MS1_MOTOR_Pin*/
    GPIO_InitStruct.Pin = MS3_MOTOR_Pin|MS2_MOTOR_Pin|MS1_MOTOR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : SLEEP_N_MOTOR_Pin RST_N_MOTOR_Pin DIR_MOTOR_Pin */
    GPIO_InitStruct.Pin = SLEEP_N_MOTOR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = RST_N_MOTOR_Pin|DIR_MOTOR_Pin|STEP_MOTOR_Pin|CW_MOTOR_Pin|CCW_MOTOR_Pin;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void stepper_init(void)
{
    stepper_gpio_hw_init();
    HAL_GPIO_WritePin(GPIOD, SLEEP_N_MOTOR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, RST_N_MOTOR_Pin, GPIO_PIN_SET);     
}

void motor_accel(motor_info_s *motorInfo)
{
  int16_t i=0;
  uint16_t tim = 0;
  
  if(motorInfo->dir == 1)
    stepper_dir_control(STEPPER_DIR_FORWARD);
  else if(motorInfo->dir == 2)
    stepper_dir_control(STEPPER_DIR_REVERSE);
  else 
    return;
  
  motorInfo->status = MOTOR_ACC;
  
  for(i=0; i<ACC_STEP_NUM; i++)
  {
     tim = AccStep[i];
     stepper_set_level_control(STEPPER_LEVEL_LOW);
     delay_us(tim);      
     stepper_set_level_control(STEPPER_LEVEL_HIGH);
     delay_us(tim);    
  }
  
  motorInfo->stepCnt += ACC_STEP_NUM;
  motorInfo->timNormal = tim;
  motorInfo->status = MOTOR_NORMAL;
}

void motor_slow(motor_info_s *motorInfo)
{
  int16_t i=0;
  uint16_t tim = 0;
  
  if(motorInfo->dir == 1)
    stepper_dir_control(STEPPER_DIR_FORWARD);
  else if(motorInfo->dir == 2)
    stepper_dir_control(STEPPER_DIR_REVERSE);
  else 
    return;
  
  motorInfo->status = MOTOR_ACC;
  for(i=ACC_STEP_NUM-1; i>=0; i--)
  {
     tim = AccStep[i];
     stepper_set_level_control(STEPPER_LEVEL_LOW);
     delay_us(tim);      
     stepper_set_level_control(STEPPER_LEVEL_HIGH);
     delay_us(tim);    
  }
  motorInfo->stepCnt -= ACC_STEP_NUM;
  motorInfo->timNormal = 0;
  motorInfo->status = MOTOR_IDLE;  
}

void stepper_sleep_control(stepper_sleep_e mode)
{
    if(mode == STEPPER_SLEEP_MODE)
        HAL_GPIO_WritePin(SLEEP_N_MOTOR_GPIO_Port, SLEEP_N_MOTOR_Pin, GPIO_PIN_RESET);
    else if(mode == STEPPER_WAKEUP_MODE)
        HAL_GPIO_WritePin(SLEEP_N_MOTOR_GPIO_Port, SLEEP_N_MOTOR_Pin, GPIO_PIN_SET);
}

void stepper_microstep_control(stepper_microstep_e mode)
{
    switch(mode)
    {
        case FULL_STEP_MODE: 
        {
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS1_MOTOR_Pin|MS2_MOTOR_Pin|MS3_MOTOR_Pin, GPIO_PIN_RESET); 
        }
        break;
        case HALF_STEP_MODE: 
        {
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS1_MOTOR_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS2_MOTOR_Pin|MS3_MOTOR_Pin, GPIO_PIN_RESET);
        }
        break;
        case QUARTER_STEP_MODE: 
        {
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS2_MOTOR_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS1_MOTOR_Pin|MS3_MOTOR_Pin, GPIO_PIN_RESET);
        } 
        break;
        case EIGHTH_STEP_MODE: 
        {
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS3_MOTOR_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS1_MOTOR_Pin|MS2_MOTOR_Pin, GPIO_PIN_SET);        
        }
        break;
        case SIXTEENTH_STEP_MODE: 
        {
            HAL_GPIO_WritePin(MS_MOTOR_GPIO_PORT, MS1_MOTOR_Pin|MS2_MOTOR_Pin|MS3_MOTOR_Pin, GPIO_PIN_SET);              
        }
        break;
        default: break;
    }
}

void stepper_dir_control(stepper_dir_e dir)
{
    if(dir == STEPPER_DIR_FORWARD)
    {
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, DIR_MOTOR_Pin, GPIO_PIN_RESET);  // 需要实际进行测试
    }
    else if(dir == STEPPER_DIR_REVERSE)
    {
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, DIR_MOTOR_Pin, GPIO_PIN_SET);  // 需要实际进行测试
    }
}

void stepper_reset_control(stepper_reset_e reset)
{
    if(reset ==  STEPPER_RESET)
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, RST_N_MOTOR_Pin, GPIO_PIN_RESET);
    else if(reset == STEPPER_NO_RESET)
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, RST_N_MOTOR_Pin, GPIO_PIN_SET);
}

void stepper_run_control(stepper_dir_e dir, uint32_t num)
{
    uint32_t i = 0;
    uint16_t tim = 0;

    if(dir == STEPPER_DIR_FORWARD)
    {
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, DIR_MOTOR_Pin, GPIO_PIN_RESET);  // 需要实际进行测试
    }
    else if(dir == STEPPER_DIR_REVERSE)
    {
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, DIR_MOTOR_Pin, GPIO_PIN_SET);  // 需要实际进行测试
    }
    
    tim = 1000;
    for(i=0; i<num; i++)
    {
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, STEP_MOTOR_Pin, GPIO_PIN_RESET);
        stepper_delay_us(tim);
        HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, STEP_MOTOR_Pin, GPIO_PIN_SET);
        stepper_delay_us(tim);        
    }
}

void stepper_set_level_control(stepper_level_e level)
{
  if(level == STEPPER_LEVEL_LOW)
    HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, STEP_MOTOR_Pin, GPIO_PIN_RESET);
  else if(level == STEPPER_LEVEL_HIGH)
    HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, STEP_MOTOR_Pin, GPIO_PIN_SET);
}

static void stepper_delay_us(uint32_t us)
{
    uint16_t i = 0;
    do
    {
        while(++i < 50000);
        i = 0;
    } while (--us);
}

void stepper_dir_led_control(stepper_dir_e dir, uint8_t status)
{
  if(dir == STEPPER_DIR_FORWARD)
    HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, CW_MOTOR_Pin, (GPIO_PinState)status); 
  else if(dir == STEPPER_DIR_REVERSE)
    HAL_GPIO_WritePin(STEP_MOTOR_GPIO_Port, CCW_MOTOR_Pin, (GPIO_PinState)status); 
}


//void stepper_walk(stepper_dir_e dir, uint16_t stepCnt, motor_info_s *motorInfo)
void stepper_walk(stepper_dir_e dir, uint16_t stepCnt)
{
  int16_t i=0;
  uint16_t tim = 0;
  uint16_t totalStepCnt = 0;
  uint16_t accStepCnt = 0;
  uint16_t slowStepCnt = 0;
  uint16_t nomalStepCnt = 0;
  
  totalStepCnt = stepCnt;
  accStepCnt = ACC_STEP_NUM;
  slowStepCnt =  ACC_STEP_NUM;
  nomalStepCnt = totalStepCnt - slowStepCnt - slowStepCnt;
  
  if(dir == 1)
    stepper_dir_control(STEPPER_DIR_FORWARD);   
  else if(dir == 2)
    stepper_dir_control(STEPPER_DIR_REVERSE);

  for(i=0; i<totalStepCnt; i++)
  {
    if(i<accStepCnt)
      tim = AccStep[i];     
    else if((i>=accStepCnt)&&(i<=(accStepCnt+nomalStepCnt)))
      tim = tim;
    else if(i>(accStepCnt+nomalStepCnt))
      tim = AccStep[totalStepCnt-i+1];
      
    stepper_set_level_control(STEPPER_LEVEL_LOW);
    delay_us(tim);      
    stepper_set_level_control(STEPPER_LEVEL_HIGH);
    delay_us(tim);     
  }
}