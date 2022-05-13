#include "systemc-ams.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

SCA_TDF_MODULE(sensor3) {

sca_tdf::sca_out<double> P;
int i;

SCA_CTOR(sensor3): P("P"), i(0){}

void set_attributes();

void initialize();

void processing();

};
