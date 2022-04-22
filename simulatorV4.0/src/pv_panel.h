#include "systemc-ams.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <string>

SCA_TDF_MODULE(pv_panel){

	sca_tdf::sca_out<double> P;  // Pactive Pidle

	int i;

	ifstream top;		     // file from which irradiance values are retrieved

	double val;

	double Gtop;

	SCA_CTOR(pv_panel):P("P") {}

	void set_attributes();
	
	void initialize();

	void processing();


	private:
	 int counter;		     // when counter % PERIOD == 0 a new measure of irradiance is read from the file

};
