//#ifndef _TSTEP
//#define _TSTEP
#define SIM_STEP 1

// The voltage of the bus
#define VREF_CTI 3.5

// Set the initial SOC value
#define SOC_INIT 0.8

// Self discharge constant factor
#define SELFDISCH_FACTOR 0.00001

//The system operating cycle period
#define PERIOD  100

//Parameters for sensor one
#define S1_ON 0.25
#define S1_ON_TIME 4
#define S1_IDLE 0.01

//Parameters for sensor two
#define S2_ON 0.15
#define S2_ON_TIME 4
#define S2_IDLE 0.01

//Parameters for mcu
#define MCU_P1 0.3
#define MCU_P2 0.15
#define MCU_T1 5
#define MCU_T2 10
#define MCU_IDLE 0.01

//Parameters for RF
#define RF_P1 0.3
#define RF_P2 0.15
#define RF_T1 7
#define RF_T2 10
#define RF_IDLE 0.01

// not in use now
// My parameters for Constant Load
#define LOAD_Pactive 2
#define LOAD_T1 50
#define LOAD_Pidle 0.1


//#endif
