#include "battery_voc.h"

void battery_voc::set_attributes(){
	out.set_timestep(SIM_STEP,sc_core::SC_SEC);
	out2.set_timestep(SIM_STEP,sc_core::SC_SEC);
	out3.set_timestep(SIM_STEP,sc_core::SC_SEC);
	//out.set_delay(1);
	out2.set_delay(1);
	out3.set_delay(1);
}

void battery_voc::initialize(){

}


void battery_voc::processing(){
	
	double tmpcurrent = in.read();			      // current requested to the battery: if negative, the battery is being supplied 
	
	tmpsoc = tmpsoc - ((tmpcurrent*SIM_STEP)/(3600*3.4)); // 3600 * Cnom, convert Ah to As due to simulation step is 1s

	// Since the battery capacity dependece on current values and frequency is omitted
	// The following three lines can be ignored.
        double deltacurrent = in2.read();
        double deltafrequency = in3.read();
	tmpsoc = tmpsoc - deltacurrent - deltafrequency;

	tmpsoc = (1.0 - SELFDISCH_FACTOR) * tmpsoc;		// at each instant the battery gets a bit discharged by itself

	// The relationship between battery SOC and battery Voc
	out.write(34.09*pow(tmpsoc,5)-91.3*pow(tmpsoc,4)+90.81*pow(tmpsoc,3)-41.1*pow(tmpsoc,2)+9.157*tmpsoc+2.584);
	// The battery internal resistance can be modified in here, now it is simplified as a fixed value
	out2.write(0.0005);
	// Output the battery SOC

	// In order not to let the SOC of the battery overflow
	if(tmpsoc >= 1) {
	    out3.write(1);
	} else {
	    out3.write(tmpsoc);
	}

	// When the battery SOC decreases to 10%, the simulation stops.	
	if(tmpsoc < 0.1){
		cout<<"SOC is less than or equal to 10%:"<<" @"<<sc_time_stamp()<<endl;
		sc_stop();
	}
	

}



