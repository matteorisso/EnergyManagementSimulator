#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(sensor2){
	sca_tdf::sca_out<double> P; // Pactive Pidle

	int i;
	
	SCA_CTOR(sensor2):P("P"),i(0) {}

	void set_attributes();
	
	void initialize();

	void processing();


};
