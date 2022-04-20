#include "sensor2.h"

void sensor2::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor2::initialize(){

}

void sensor2::processing(){
		
 //if(i==PERIOD){
 //  i = 0;
 //}else {  
   if(i<=S2_ON_TIME){
     P.write(S2_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(S2_IDLE);
     i = (i+1) % PERIOD;
   }
  
//}

}
