#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(converter){
	sca_tdf::sca_in<double> in; // Total power should be provided battery
	sca_tdf::sca_in<double> in2; // Battery current
	sca_tdf::sca_out<double> out;// Battery current

	SCA_CTOR(converter){}

	void set_attributes();

	void initialize();

	// Processes to calculate converter efficiency and current(Battery load current)
	void processing();
};
