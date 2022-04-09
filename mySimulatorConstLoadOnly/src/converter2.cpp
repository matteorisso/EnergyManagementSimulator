#include "converter2.h"

void converter2::set_attributes()
{
  out.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void converter2::initialize(){

}

void converter2::processing()
{
	double tmp_power = in.read();
	double tmp_vout = VREF_CTI;

	// Assume a fixed effciency.
	// Same as converter1.
	double efficiency = 0.95;
        out.write(tmp_power*efficiency/tmp_vout);
}
