#include "sensor1.h"

void sensor1::set_attributes(){
 done.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){

}

void sensor1::processing(){

	if(enable.read() == 1) {
	    data.write(18.0);
            done.write(1);
	    cout << "sensor1 is enabled!" << endl;
        } else {
	    data.write(0.0);
            done.write(0);	
	    cout << "sensor1 is NOT enabled!" << endl;
	}

/*
      if(i >= sensor1_ACT_TIME && i< sensor1_ACT_TIME + sensor1_ON_TIME){
     P.write(sensor1_ON);
     i = (i+1) % PERIOD;
   }else {
     P.write(sensor1_IDLE);
     i = (i+1) % PERIOD;
   }
*/
}
