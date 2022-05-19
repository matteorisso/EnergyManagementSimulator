#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(mcu){
	sca_tdf::sca_out<double> P; // Pactive Pidle
	sca_tdf::sca_out<bool> enable1;
	sca_tdf::sca_in<double> measure1;
	sca_tdf::sca_in<bool> done1;

	int i;

	SCA_CTOR(mcu):P("P"),i(0), enable1("enable1"), measure1("measure1"), done1("done1"){}

	void set_attributes();
	
	void initialize();

	void processing();


};
