#include "systemc-ams.h"
#include "config.h"
#define TIMINGWIN 32 // For using the STFT when compute the deltaf. ignore it when using the simple battery model

SCA_TDF_MODULE(battery_fre){
	sca_tdf::sca_in<double> in,in2; //Battery current,Battery SOC
	sca_tdf::sca_out<double> out,out2;//deltaf,deltac


	double tmpcurrent,tmpsoc;

	SCA_CTOR(battery_fre):out("out"),out2("out2"){}

	void set_attributes();

	void initialize();

	void processing();


};
