#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(mcu){

	int i;
	int rf_act_time_in_use_tmp;

	sca_tdf::sca_out<int> enableSensor1;
	sca_tdf::sca_in<int> doneSensor1;
	sca_tdf::sca_in<double> readSensor1;

	SCA_CTOR(mcu):i(0), enableSensor1("enableSensor1"), doneSensor1("doneSensor1"), readSensor1("readSensor1"){}

	void set_attributes();
	
	void initialize();

	void processing();


};
