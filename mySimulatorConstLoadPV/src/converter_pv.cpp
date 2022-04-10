#include "converter_pv.h"

void converter_pv::set_attributes()
{
  out.set_timestep(SIM_STEP, sc_core::SC_SEC);
  out.set_delay(1);
}

void converter_pv::initialize(){

}

void converter_pv::processing()
{
    double tmp_power = in.read();
    double tmp_vout = VREF_CTI;
	
    double efficiency_pv = 0.8;     // PV panel efficiency, could be bad tracking MPPT
    out.write(tmp_power*efficiency_pv/tmp_vout);
}


