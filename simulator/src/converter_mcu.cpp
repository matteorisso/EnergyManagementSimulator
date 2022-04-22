#include "converter_mcu.h"

void converter_mcu::set_attributes()
{
  out.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void converter_mcu::initialize(){

}

void converter_mcu::processing()
{
	double tmp_power = in.read();
	double tmp_vout = VREF_CTI;

	// Assume a fixed effciency.
	// Same implementation as sensor's converter.
	double efficiency = 0.95;
        out.write(tmp_power*efficiency/tmp_vout);
}
