//#ifndef _TSTEP
//#define _TSTEP

#define ALL_SENSORS

#define SIM_STEP 1

// The voltage of the bus
#define VREF_CTI 3.5

// Set the initial SOC value
#define SOC_INIT 0.8

// Self discharge constant factor
#define SELFDISCH_FACTOR 0.00001

//The system operating cycle period
#define PERIOD  100


#ifdef ALL_SENSORS
//Parameters for sensor one
#define S1_ON 1
#define S1_ON_TIME 40
#define S1_IDLE 0.01
#define S1_ACT_TIME 0

//Parameters for sensor two
#define S2_ON 0.8
#define S2_ON_TIME 30
#define S2_IDLE 0.01
#define S2_ACT_TIME 0

//Parameters for mcu
#define MCU_P1 2.2
#define MCU_P2 1.4
#define MCU_T1 40
#define MCU_T2 30
#define MCU_IDLE 0.01
#if ((S1_ACT_TIME + S1_ON_TIME) >= (S2_ACT_TIME + S2_ON_TIME))
    #define MCU_ACT_TIME (S1_ACT_TIME + S1_ON_TIME)
#else 
    #define MCU_ACT_TIME (S2_ACT_TIME + S2_ON_TIME)
#endif

//Parameters for RF
#define RF_P1 2
#define RF_P2 1.2
#define RF_T1 3
#define RF_T2 5
#define RF_IDLE 0.01
#define RF_ACT_TIME (MCU_ACT_TIME + MCU_T1)  // FIX THIS !!

#endif

#ifndef ALL_SENSORS
// My parameters for Constant Load
#define LOAD_Pactive 1.1
#define LOAD_T1 50
#define LOAD_Pidle 0.1

#endif

//#endif
