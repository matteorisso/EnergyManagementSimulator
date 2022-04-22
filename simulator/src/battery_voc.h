#include "systemc-ams.h"
#include "config.h"

SCA_TDF_MODULE(battery_voc){
	sca_tdf::sca_in<double> in,in2,in3; // Battery current; Vlost for rated capacity; Vlost for load frequency
	sca_tdf::sca_out<double> out,out2,out3;//Voc; Rs; SOC 
	

	SCA_CTOR(battery_voc):out("out"),out2("out2"),out3("out3"),tmpsoc(SOC_INIT){} // The initial SOC value is set to 80%, can be changed to any value in the config.h file

	void set_attributes();

	void initialize();

	void processing();

	private:
	double tmpsoc;


};
