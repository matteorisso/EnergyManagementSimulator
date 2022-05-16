#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(rf){
	sca_tdf::sca_out<double> P; // Pactive Pidle
	sca_tdf::sca_in<double> SOC; // SOC signal
	sca_tdf::sca_in<int> rf_act_time_in_use;

	int i;
	double tmpsoc;
	int rf_act_time_in_use_tmp;

	SCA_CTOR(rf):P("P"),i(0),tmpsoc(SOC_INIT), rf_act_time_in_use("rf_act_time_in_use") {}

	void set_attributes();
	
	void initialize();

	void processing();


};
