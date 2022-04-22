#include "battery_fre.h"

void battery_fre::set_attributes(){
	out.set_timestep(SIM_STEP,sc_core::SC_SEC);
	out2.set_timestep(SIM_STEP,sc_core::SC_SEC);
}

void battery_fre::initialize(){

}


void battery_fre::processing(){

	tmpcurrent = in.read();
	tmpsoc = in2.read();

	//The capacity dependence on the load current value and frequency is computed in here.
	//Set these two values to 0 means that there is no rated capacity effect and dependence on load frequency.
	out.write(0.0);
	out2.write(0.0);


}
