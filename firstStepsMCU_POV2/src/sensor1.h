#include "systemc-ams.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

SCA_TDF_MODULE(sensor1) {

int i;
sca_tdf::sca_in<int> enable;
sca_tdf::sca_out<double> data;		// data to be output
sca_tdf::sca_out<int> done;

SCA_CTOR(sensor1): i(0), enable("enable"), data("data"), done("done"){}

void set_attributes();

void initialize();

void processing();

};
