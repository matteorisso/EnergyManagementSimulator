#include "sensor1.h"

// uso questo sensore come constant load

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){
}

void sensor1::processing(){

 if(i==PERIOD){
   i = 0;
 }else {  
   if(i<=LOAD_T1){
     P.write(LOAD_Pactive);
     i++;
   }else {
     P.write(LOAD_Pidle);
     i++;
   }
 }


}
