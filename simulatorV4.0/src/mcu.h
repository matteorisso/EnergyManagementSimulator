#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(mcu){
	sca_tdf::sca_out<double> P; // Pactive Pidle
	sca_tdf::sca_in<double> SOC; // SOC signale

	int i;
	double tmpsoc;	

	SCA_CTOR(mcu):P("P"),i(0),tmpsoc(SOC_INIT){}

	void set_attributes();
	
	void initialize();

	void processing();


};
