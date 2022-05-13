#include "sensor1.h"

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

void sensor1::processing(){

      if(i >= sensor1_ACT_TIME && i< sensor1_ACT_TIME + sensor1_ON_TIME){
     P.write(sensor1_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(sensor1_IDLE);
     i = (i+1) % PERIOD;
   }

}