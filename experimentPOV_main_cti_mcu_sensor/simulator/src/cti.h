#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE (cti)
{

double retrievedData;

sca_tdf::sca_in<int> CTItoMCU_enS1, CTItoS1_done;
sca_tdf::sca_in<double> CTItoS1_data;
sca_tdf::sca_out<double>  CTItoMCU_dataS1;
sca_tdf::sca_out<int>  CTItoMCU_doneS1, CTItoS1_en;

SCA_CTOR(cti): doneSensor1_S1("CTItoS1_done"), enableSensor1_S1("CTItoS1_en"), dataSensor1_S1("CTItoS1_data"), doneSensor1_MCU("CTItoMCU_doneS1"), \
enableSensor1_MCU("CTItoMCU_enS1"), dataSensor1_MCU("CTItoMCU_dataS1"){};


void set_attributes();

void initialize();

void processing();

};
