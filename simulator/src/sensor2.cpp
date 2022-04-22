#include "sensor2.h"

void sensor2::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor2::initialize(){

}

void sensor2::processing(){
		
   // this module is not inserted if ALL_SENSORS is not defined in "src/config.h"

   #ifdef ALL_SENSORS
      if(i >= S2_ACT_TIME && i< S2_ACT_TIME + S2_ON_TIME){
     P.write(S2_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(S2_IDLE);
     i = (i+1) % PERIOD;
   }
   #endif

}
