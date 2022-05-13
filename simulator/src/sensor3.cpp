#include "sensor3.h"

void sensor3::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor3::initialize(){

}

void sensor3::processing(){

      if(i >= sensor3_ACT_TIME && i< sensor3_ACT_TIME + sensor3_ON_TIME){
     P.write(sensor3_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(sensor3_IDLE);
     i = (i+1) % PERIOD;
   }

}