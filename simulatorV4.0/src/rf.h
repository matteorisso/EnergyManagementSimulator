#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(rf){
	sca_tdf::sca_out<double> P; // Pactive Pidle
	sca_tdf::sca_in<double> SOC; // SOC signal

	int i;
	double tmpsoc;	

	SCA_CTOR(rf):P("P"),i(0),tmpsoc(SOC_INIT) {}

	void set_attributes();
	
	void initialize();

	void processing();


};
