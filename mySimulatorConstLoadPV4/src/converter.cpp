#include "converter.h"

void converter::set_attributes()
{
  out.set_timestep(SIM_STEP, sc_core::SC_SEC);
  out.set_delay(1);
}

void converter::initialize(){

}

void converter::processing()
{
	double tmp_power = in.read();
	double tmp_vout = in2.read();

	// Assume a fixed effciency.
	// Change in here if want to have an advanced converter model with dynamic efficiency.
	double efficiency = 0.95;
        out.write(tmp_power*efficiency/tmp_vout);
}
