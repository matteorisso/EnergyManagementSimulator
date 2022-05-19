#include "mcu.h"
#include "sensor1.h"

void mcu::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){
}

void mcu::processing(){


    if(i == 0){
        enable1.write(true);
        cout << "sensor1.enable.write(true);" << endl;
    } else {
        enable1.write(false);
        cout << "sensor1.enable.write(false);" << endl;
    }

    if(done1.read() == 1){
        P.write(measure1.read());
        cout << "ho letto il valore " << measure1.read() << " al tempo " << i << endl;
    }

    P.write(1.0);

    i = (i + 1) % 100;
}
