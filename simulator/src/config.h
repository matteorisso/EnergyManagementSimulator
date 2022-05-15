// generic simulator params
#define SIM_STEP 1
#define VREF_CTI 3.5
#define SOC_INIT 0.8
#define SELFDISCH_FACTOR 1e-05
#define PERIOD 100

// params for sensor sensor1
#define sensor1_ON 1
#define sensor1_ON_TIME 15
#define sensor1_IDLE 0.01
#define sensor1_ACT_TIME 10

// params for sensor sensor2
#define sensor2_ON 1
#define sensor2_ON_TIME 3
#define sensor2_IDLE 0.01
#define sensor2_ACT_TIME 12

// params for sensor sensor3
#define sensor3_ON 1
#define sensor3_ON_TIME 3
#define sensor3_IDLE 0.01
#define sensor3_ACT_TIME 12

// params for sensor sensor4
#define sensor4_ON 1
#define sensor4_ON_TIME 3
#define sensor4_IDLE 0.01
#define sensor4_ACT_TIME 12

// params for MCU
#define MCU_IDLE 0.01
#define MCU_ACT_TIME 26

#define MCU_full_speed_POWER_ON 0.7
#define MCU_full_speed_TIME_ON 20
#define MCU_full_speed_THRESHOLD 0.7

#define MCU_medium_speed_POWER_ON 0.4
#define MCU_medium_speed_TIME_ON 25
#define MCU_medium_speed_THRESHOLD 0.4

#define MCU_low_speed_POWER_ON 0.2
#define MCU_low_speed_TIME_ON 30
#define MCU_low_speed_THRESHOLD 0

// params for RF
#define RF_P1 2
#define RF_P2 1.2
#define RF_T1 10
#define RF_T2 20
#define RF_IDLE 0.01
#define RF_ACT_TIME1 1
#define RF_ACT_TIME2 1