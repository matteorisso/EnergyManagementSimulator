#include "sensor1.h"

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

void processing(){

  enable_tmp = enable.read()


  if(enable.read() == 1){
    measure_tmp = 18.0;
    measure.write(measure_tmp);
    //wait(20, sc_core::SC_SEC);
    // P.write(2.0);
    P.write(1);
    done_tmp = 1;
    done.write(done_tmp);
  }

}
