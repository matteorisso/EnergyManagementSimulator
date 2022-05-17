#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(mcu){
	sca_tdf::sca_out<double> P; // Pactive Pidle

	int i;

    sca_tdf::sca_in<bool> enable1;
    sca_tdf::sca_out<double> measure1;
    sca_tdf::sca_out<bool> done1;

	SCA_CTOR(mcu):P("P"),i(0){}

	void set_attributes();
	
	void initialize();

	void processing();


};
