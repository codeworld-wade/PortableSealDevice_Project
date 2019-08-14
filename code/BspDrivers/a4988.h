/*
 *  a4988 stepper driver file
 */

#ifndef __A4988_H
#define __A4988_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "stm32f1xx_hal.h"
//#include "bsp_conf.h"


#define MOTOR_LED_ON 0
#define MOTOR_LED_OFF 1
  
#define MOTOR_STEP_CNT_OFFSET 20000
#define ACC_STEP_NUM 1000 
   
#define ACC_RATE 3      // һ�μƲ������У����ٱ���
#define SLOW_RATE 3     // һ�μƲ������У����ٱ�����
#define TOTAL_RATE 10   // һ�μƲ������У��ܹ�����������
  
typedef enum {
    STEPPER_SLEEP_MODE,     // �������ģʽ
    STEPPER_WAKEUP_MODE,    // �������ģʽ
    STEPPER_UNKNOWN
} stepper_sleep_e;

typedef enum {
    FULL_STEP_MODE,         // ȫ����
    HALF_STEP_MODE,         // �벽��
    QUARTER_STEP_MODE,      // 1/4����
    EIGHTH_STEP_MODE,       // 1/8����
    SIXTEENTH_STEP_MODE,    // 1/16����
} stepper_microstep_e;

typedef enum {
    STEPPER_DIR_INVALID,
    STEPPER_DIR_FORWARD,
    STEPPER_DIR_REVERSE
} stepper_dir_e;

typedef enum {
    STEPPER_RESET,
    STEPPER_NO_RESET
} stepper_reset_e;

typedef enum {
  STEPPER_LEVEL_LOW,
  STEPPER_LEVEL_HIGH,
} stepper_level_e;

typedef enum {
  MOTOR_IDLE,
  MOTOR_ACC,
  MOTOR_SLOW,
  MOTOR_NORMAL
} motor_status_e;
     

typedef struct {
  uint16_t accFlag;     // 0-�޼���Ҫ��1-����Ҫ��
  uint16_t slowFlag;    // 0-�޼���Ҫ��1-����Ҫ��
  uint8_t status;       // 0-idle; 1-acc; 2-slow; 3-normal
  uint8_t dir;          // �������
  uint16_t accStepCnt;  // ���ٲ���
  uint16_t slowStepCnt; // ���ٲ���
  uint16_t stepCnt;     // �Ʋ��������̶�ƫ��ֵ20000
  uint16_t timNormal;   // ������ʱʱ��
  uint16_t stepCntTarget;// Ŀ����ٲ���
} motor_info_s;

extern unsigned short AccStep[ACC_STEP_NUM];
  
extern void stepper_init(void);
//extern void stepper_speedUp_calculate(void);
//extern void stepper_slowDown_calculate(void);

extern void motor_accel(motor_info_s *motorInfo);
extern void motor_slow(motor_info_s *motorInfo);
extern void stepper_sleep_control(stepper_sleep_e mode);
extern void stepper_microstep_control(stepper_microstep_e mode);
extern void stepper_dir_control(stepper_dir_e dir);
extern void stepper_reset_control(stepper_reset_e reset);
extern void stepper_run_control(stepper_dir_e dir, uint32_t num);
extern void stepper_set_level_control(stepper_level_e level);
extern void stepper_dir_led_control(stepper_dir_e dir, uint8_t status);
//extern void stepper_walk(stepper_dir_e dir, uint16_t stepCnt, motor_info_s *motorInfo);
extern void stepper_walk(stepper_dir_e dir, uint16_t stepCnt);
#ifdef __cplusplus 
}
#endif


#endif