#include "sensor4.h"

void sensor4::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor4::initialize(){

}

void sensor4::processing(){

if(i >= S4_ACT_TIME && i< S4_ACT_TIME + S4_ON_TIME){
  P.write(S4_ON);
  i = (i+1) % PERIOD;
}else{
  P.write(S4_IDLE);
  i = (i+1) % PERIOD;
}

}

