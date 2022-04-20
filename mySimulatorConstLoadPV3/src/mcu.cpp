#include "mcu.h"

void mcu::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){

}

void mcu::processing(){
	
	tmpsoc = SOC.read();

	//if(i == PERIOD){
	//	i = 0.0;
	//}else { // Simple policy: 
		// In the active state: battery SOC > 50%, MCU in fully active state; othervise, MCU in semi active state.
		// In the idle state: MCU consumes MCU_IDLE power 
		// Modify the operating values in config.h file. 
		if(tmpsoc>0.5){
			if(i<=MCU_T1){
			P.write(MCU_P1);
			i = (i+1) % PERIOD;
			}else{
			P.write(MCU_IDLE);
			i = (i+1) % PERIOD;
			}
		}else{
			if(i<=MCU_T2){
			P.write(MCU_P2);
			i = (i+1) % PERIOD;
			}else{
			P.write(MCU_IDLE);
			i = (i+1) % PERIOD;
			}
		}
	//}

}
