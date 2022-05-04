#include "sensor1.h"

// ATTENTION !
// This sensor is used as a regular sensor if ALL_SENSORS is defined in "src/config.h"
// This sensor is used as a generic load if ALL_SENSORS is NOT defined in "src/config.h" 

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

void sensor1::processing(){

   if(i >= S1_ACT_TIME && i< S1_ACT_TIME + S1_ON_TIME){
     P.write(S1_ON);
     i = (i + 1) % PERIOD;
   }else {
     P.write(S1_IDLE);
     i = (i+1) % PERIOD;
   }

}
