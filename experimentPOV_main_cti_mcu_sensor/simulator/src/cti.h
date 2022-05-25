#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE (cti)
{

double retrievedData;

sca_tdf::sca_in<int> enableSensor1_MCU, doneSensor1_S1;
sca_tdf::sca_in<double> dataSensor1_S1;
sca_tdf::sca_out<double>  dataSensor1_MCU;
sca_tdf::sca_out<int>  doneSensor1_MCU, enableSensor1_S1;

SCA_CTOR(cti): doneSensor1_S1("doneSensor1_S1"), enableSensor1_S1("enableSensor1_S1"), \
dataSensor1_S1("dataSensor1_S1"), doneSensor1_MCU("doneSensor1_MCU"), \
enableSensor1_MCU("enableSensor1_MCU"), dataSensor1_MCU("dataSensor1_MCU"){};


void set_attributes();

void initialize();

void processing();

};
