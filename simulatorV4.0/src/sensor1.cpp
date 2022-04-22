#include "sensor1.h"

// ATTENTION !
// This sensor is used as a regular sensor if ALL_SENSORS is defined in "src/config.h"
// This sensor is used as a generic load if ALL_SENSORS is NOT defined in "src/config.h" 

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){
    #ifndef ALL_SENSORS
        srand(time(NULL));
    #endif
}

void sensor1::processing(){
   
   #ifdef ALL_SENSORS
   if(i >= S1_ACT_TIME && i< S1_ACT_TIME + S1_ON_TIME){
     P.write(S1_ON);
     i = (i + 1) % PERIOD;
   }else {
     P.write(S2_IDLE);
     i = (i+1) % PERIOD;
   }
   #endif

   #ifndef ALL_SENSORS

   if(i == 0) {
       randomPicker = rand() % 11;		// 11 possible duty cycle values, 0% : 10 : 100%
       switch(randomPicker) {
           case 0:
		CURRENT_DUTY_CYCLE = LOAD_T0;
                break;
           case 1:
		CURRENT_DUTY_CYCLE = LOAD_T10;
                break;
           case 2:
		CURRENT_DUTY_CYCLE = LOAD_T20;
                break;
           case 3:
		CURRENT_DUTY_CYCLE = LOAD_T30;
                break;
           case 4:
		CURRENT_DUTY_CYCLE = LOAD_T40;
                break;
           case 5:
		CURRENT_DUTY_CYCLE = LOAD_T50;
                break;
           case 6:
		CURRENT_DUTY_CYCLE = LOAD_T60;
                break;
           case 7:
		CURRENT_DUTY_CYCLE = LOAD_T70;
                break;
           case 8:
		CURRENT_DUTY_CYCLE = LOAD_T80;
                break;
           case 9:
		CURRENT_DUTY_CYCLE = LOAD_T90;
                break;
           case 10:
		CURRENT_DUTY_CYCLE = LOAD_T100;
                break;
           default:
		CURRENT_DUTY_CYCLE = LOAD_T50;
                break;
       }
   }
	
   if(i<CURRENT_DUTY_CYCLE){
     P.write(LOAD_Pactive);
     i = (i + 1) % PERIOD;
   }else {
     P.write(LOAD_Pidle);
     i = (i+1) % PERIOD;
   }   
   #endif

}
