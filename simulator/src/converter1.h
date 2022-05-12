#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(converter1) {

sca_tdf::sca_in<double> in;
sca_tdf::sca_out<double> out;

SCA_CTOR(converter1){}

void set_attributes();

void initialize();

void processing();

};

