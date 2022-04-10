#include "pv_panel.h"

void pv_panel::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void pv_panel::initialize(){
}

void pv_panel::processing(){

    P.write(0.4);

}
