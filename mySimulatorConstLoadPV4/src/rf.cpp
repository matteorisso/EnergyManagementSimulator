#include "rf.h"

void rf::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void rf::initialize(){

}

void rf::processing(){

	// Starting point for you.
	// First try: Modify below codes,to keep the RF have the mcu's similar implementation
	tmpsoc = SOC.read();

	//cout << "RF_ACT_TIME " << RF_ACT_TIME << endl;

	//if(i == PERIOD){
	//	i = 0.0;
	//}else {
                #ifdef ALL_SENSORS
		if(tmpsoc>0.5){
		      if(i >= RF_ACT_TIME && i < RF_ACT_TIME + RF_T1){
		          P.write(RF_P1);
		          i = (i+1) % PERIOD;
			  // cout << "rf active grande at " << i << endl;
		      }else{
		          P.write(RF_IDLE);
		          i = (i+1) % PERIOD;
		      }
		    }else{
		      if(i >= RF_ACT_TIME && i < RF_ACT_TIME + RF_T2){
		          P.write(RF_P2);
		          i = (i+1) % PERIOD;
                          // cout << "rf active piccolo at " << i << endl;
		      }else{
		          P.write(RF_IDLE);
		          i = (i+1) % PERIOD;
		      }
		    }
                #endif
	//	}
}
