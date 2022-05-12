#include "sensor3.h"

void sensor3::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor3::initialize(){

}

void sensor3::processing(){

if(i >= S3_ACT_TIME && i< S3_ACT_TIME + S3_ON_TIME){
  P.write(S3_ON);
  i = (i+1) % PERIOD;
}else{
  P.write(S3_IDLE);
  i = (i+1) % PERIOD;
}

}

