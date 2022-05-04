#include "systemc-ams.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

SCA_TDF_MODULE(sensor1){
	sca_tdf::sca_out<double> P; // Pactive Pidle

	int i;
	
	SCA_CTOR(sensor1):P("P"),i(0) {}

	void set_attributes();
	
	void initialize();

	void processing();

};
