#include "sensor2.h"

void sensor2::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor2::initialize(){

}

void sensor2::processing(){

      if(i >= sensor2_ACT_TIME && i< sensor2_ACT_TIME + sensor2_ON_TIME){
     P.write(sensor2_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(sensor2_IDLE);
     i = (i+1) % PERIOD;
   }

}