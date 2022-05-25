#include "cti.h"


void cti::set_attributes(){}

void cti::initialize(){}

void cti::processing(){

	
	if(enableSensor1_MCU.read() == 1) {
	    enableSensor1_S1.write(1);
	    while(doneSensor1_S1.read() == 0) {}
	    retrievedData = dataSensor1_S1.read();
	    dataSensor1_MCU.write(retrievedData);
	    doneSensor1_MCU.write(1);     	
	} else {
	    dataSensor1_MCU.write(0.0);
	    doneSensor1_MCU.write(0);     	    	
	}

}
