#include "converter_rf.h"

void converter_rf::set_attributes()
{
  out.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void converter_rf::initialize(){

}

void converter_rf::processing()
{
	double tmp_power = in.read();
	double tmp_vout = VREF_CTI;

	// Assume a fixed effciency
	double efficiency = 0.95;
        out.write(tmp_power*efficiency/tmp_vout);
}
