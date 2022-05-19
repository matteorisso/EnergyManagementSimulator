#include "mcu.h"
#include "sensor1.h"

void mcu::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){
}

void mcu::processing(){


    if(i == 0){
        sensor1.enable.write(true);
        cout << "sensor1.enable.write(true);" << endl;
    } else {
        sensor1.enable.write(false);
        cout << "sensor1.enable.write(false);" << endl;
    }

    if(sensor1.done.read()){
        P.write(sensor1.measure.read());
        cout << "ho letto il valore " << sensor1.measure.read() << " al tempo " << i << endl;
    }

    P.write(1.0);

    i = (i + 1) % 100;
}