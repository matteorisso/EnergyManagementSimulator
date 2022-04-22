#include "mcu.h"

void mcu::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){

}

void mcu::processing(){


	// this module is cut out of the simulator if ALL_SENSORS is not defined in "src/config.h"
        // this module has a high power state and a low power state. If the SOC of the battery is below 50%, then the MCU enters the low power state.
	
	tmpsoc = SOC.read();

        #ifdef ALL_SENSORS
	if(tmpsoc>0.5){							// @todo: rendilo parametrizzabile 
		if(i >= MCU_ACT_TIME && i< MCU_ACT_TIME + MCU_T1){
		P.write(MCU_P1);
		i = (i+1) % PERIOD;
		}else{
		P.write(MCU_IDLE);
		i = (i+1) % PERIOD;
		}
	}else{
		if(i >= MCU_ACT_TIME && i< MCU_ACT_TIME + MCU_T2){
		P.write(MCU_P2);
		i = (i+1) % PERIOD;
		}else{
		P.write(MCU_IDLE);
		i = (i+1) % PERIOD;
		}
	}
        #endif

}
