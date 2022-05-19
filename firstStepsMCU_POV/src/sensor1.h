#include "systemc-ams.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

//  in: enable;
// out: measure, done;

SCA_TDF_MODULE(sensor1) {

	sca_tdf::sca_out<double> P;
	int i;

	int enable_tmp, done_tmp;

	double measure_tmp;

	sca_tdf::sca_in<int> enable;
	sca_tdf::sca_out<double> measure;
	sca_tdf::sca_out<int> done;


	SCA_CTOR(sensor1):P("P"),enable("enable"),measure("measure"),done("done"),i(0){}

	void set_attributes();

	void initialize();

	void processing();

};
