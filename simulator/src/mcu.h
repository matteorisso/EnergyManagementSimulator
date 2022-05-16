#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(mcu){
	sca_tdf::sca_out<double> P; // Pactive Pidle
	sca_tdf::sca_in<double> SOC; // SOC signale
	sca_tdf::sca_out<int> rf_act_time_in_use; // Vref

	int i;
	double tmpsoc;	
	int rf_act_time_in_use_tmp;

	SCA_CTOR(mcu):P("P"),i(0),tmpsoc(SOC_INIT), rf_act_time_in_use("rf_act_time_in_use"){}

	void set_attributes();
	
	void initialize();

	void processing();


};
