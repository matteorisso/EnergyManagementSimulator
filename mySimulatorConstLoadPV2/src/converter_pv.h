#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(converter_pv){
	sca_tdf::sca_in<double> in;     // Power from PV panel
	sca_tdf::sca_out<double> out;   // Current to the BUS

	SCA_CTOR(converter_pv){}

	void set_attributes();

	void initialize();

	void processing();
};
