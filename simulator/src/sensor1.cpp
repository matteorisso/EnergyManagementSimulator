#include "sensor1.h"

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

bool sensor1::processing(bool enable, double* data){

  fakeData = 18.0;

  if(enable){
    P.write(rand()%100);
    return true;
  }

}