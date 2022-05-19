#include "mcu.h"

void mcu::set_attributes(){
 enableSensor1.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){

}

void mcu::processing(){

    if(i == 0) {
	enableSensor1.write(1);
        cout << "enabled sensor1!" << endl;
	//while(doneSensor1.read() != 1) {
	//    cout << "waiting for sensor1 to finish" << endl;	
	//}
	cout << "sensed val: " << readSensor1.read() << endl;
        while(1){}
    }

    i = (i+1) % PERIOD;

}
