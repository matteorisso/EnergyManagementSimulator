#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(converter_rf){
	sca_tdf::sca_in<double> in; // Powerin, Vout
	sca_tdf::sca_out<double> out; //Iout

	SCA_CTOR(converter_rf){}

	void set_attributes();

	void initialize();

	// Processes to calculate converter efficiency and output current
	void processing();
};
