#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(converter_mcu){
	sca_tdf::sca_in<double> in; // Powerin
	sca_tdf::sca_out<double> out;//Iout

	SCA_CTOR(converter_mcu){}

	void set_attributes();

	void initialize();

	// Processes to calculate converter efficiency and output current
	void processing();
};
