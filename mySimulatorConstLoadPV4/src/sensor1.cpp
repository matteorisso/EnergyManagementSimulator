#include "sensor1.h"

// uso questo sensore come constant load

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){
}

void sensor1::processing(){

 //if(i==PERIOD){
 //  i = 0;
 //}else {  
   
   #ifdef ALL_SENSORS
   if(i >= S1_ACT_TIME && i< S1_ACT_TIME + S1_ON_TIME){
     P.write(S1_ON);
     i = (i + 1) % PERIOD;
   }else {
     P.write(S2_IDLE);
     i = (i+1) % PERIOD;
   }
   #endif
   #ifndef ALL_SENSORS
   if(i<LOAD_T1){
     P.write(LOAD_Pactive);
     i = (i + 1) % PERIOD;
   }else {
     P.write(LOAD_Pidle);
     i = (i+1) % PERIOD;
   }   
   #endif
 //}


 // prima era un po' sfasato, ora fixato ma forse mi causera' problemi in futuro. Lascio parti commentate.

}
