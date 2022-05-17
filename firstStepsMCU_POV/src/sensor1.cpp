#include "sensor1.h"

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

void processing(bool en, double &measure_out, bool &done_out){

  if(en){
    measure_out = 18.0;
    //wait(20, sc_core::SC_SEC);
    P.write(2.0);
    done_out = true;
  }

}