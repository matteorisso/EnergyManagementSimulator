#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(pv_panel){
	sca_tdf::sca_out<double> P; // Pactive Pidle

	// int i;
	
	SCA_CTOR(pv_panel):P("P") {}

	void set_attributes();
	
	void initialize();

	void processing();


};
