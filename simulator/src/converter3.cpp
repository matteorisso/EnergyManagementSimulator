#include "converter3.h"

void converter3::set_attributes(){
 out.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void converter3::initialize(){

}

void converter3::processing(){

  double tmp_power = in.read();
  double tmp_vout = VREF_CTI;

  double efficiency = 1;

  out.write(tmp_power*efficiency/tmp_vout);

}
