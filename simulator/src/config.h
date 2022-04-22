//#define ALL_SENSORS
//#define PARALLEL
#define DETERMINISTIC_LOAD

#define SIM_STEP 1

// The voltage of the bus
#define VREF_CTI 3.5

// Set the initial SOC value
#define SOC_INIT 0.8

// Self discharge factor
#define SELFDISCH_FACTOR 0.00001

//The system operating cycle period
#define PERIOD  100


#ifdef ALL_SENSORS
	//Parameters for sensor one
	#define S1_ON 1
	#define S1_ON_TIME 20
	#define S1_IDLE 0.01
	#define S1_ACT_TIME 0

	//Parameters for sensor two
	#define S2_ON 0.8
	#define S2_ON_TIME 10
	#define S2_IDLE 0.01
	#ifdef PARALLEL
	    #define S2_ACT_TIME 0
	#endif
	#ifndef PARALLEL
	    #define S2_ACT_TIME (S1_ACT_TIME + S1_ON_TIME)
	#endif

	//Parameters for mcu
	#define MCU_P1 2.2
	#define MCU_P2 1.4
	#define MCU_T1 40
	#define MCU_T2 30
	#define MCU_IDLE 0.01
	#ifdef PARALLEL
		#if ((S1_ACT_TIME + S1_ON_TIME) >= (S2_ACT_TIME + S2_ON_TIME))
		    #define MCU_ACT_TIME (S1_ACT_TIME + S1_ON_TIME)
		#else 
		    #define MCU_ACT_TIME (S2_ACT_TIME + S2_ON_TIME)
		#endif
	#endif
	#ifndef PARALLEL
		#define MCU_ACT_TIME (S2_ACT_TIME + S2_ON_TIME)
	#endif

	//Parameters for RF
	#define RF_P1 2
	#define RF_P2 1.2
	#define RF_T1 10
	#define RF_T2 20
	#define RF_IDLE 0.01
	#define RF_ACT_TIME1 (MCU_ACT_TIME + MCU_T1)
	#define RF_ACT_TIME2 (MCU_ACT_TIME + MCU_T2)

#endif

#ifndef ALL_SENSORS
	// My parameters for Constant Load
		#define LOAD_Pactive 1
	#ifndef DETERMINISTIC_LOAD
		#define LOAD_T0 0			// Pc_min
		#define LOAD_T10 10
		#define LOAD_T20 20
		#define LOAD_T30 30
		#define LOAD_T40 40
		#define LOAD_T50 50
		#define LOAD_T60 60
		#define LOAD_T70 70
		#define LOAD_T80 80
		#define LOAD_T90 90
		#define LOAD_T100 100			// Pc_MAX
	#endif
		#define LOAD_Pidle 0.1
		#define LOAD_Tdefault 50
#endif

//#endif
