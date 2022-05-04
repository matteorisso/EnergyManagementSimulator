#include "rf.h"

void rf::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void rf::initialize(){

}

void rf::processing(){

	// this module is cut out of the simulator if ALL_SENSORS is not defined in "src/config.h"
        // this module has a high power state and a low power state. If the SOC of the battery is below 50%, then the RF enters the low power state.

	tmpsoc = SOC.read();


	if(tmpsoc>0.5){							// @todo: rendilo parametrizzabile
	      if(i >= RF_ACT_TIME1 && i < RF_ACT_TIME1 + RF_T1){
	          P.write(RF_P1);
	          i = (i+1) % PERIOD;
	      }else{
	          P.write(RF_IDLE);
	          i = (i+1) % PERIOD;
	      }
	    }else{
	      if(i >= RF_ACT_TIME2 && i < RF_ACT_TIME2 + RF_T2){
	          P.write(RF_P2);
	          i = (i+1) % PERIOD;
	      }else{
	          P.write(RF_IDLE);
	          i = (i+1) % PERIOD;
	      }
	    }

}
