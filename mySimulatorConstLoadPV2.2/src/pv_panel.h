#include "systemc-ams.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <string>

// using namespace std;

SCA_TDF_MODULE(pv_panel){

	sca_tdf::sca_out<double> P; // Pactive Pidle

	int i;

	ifstream top;

	double val;
        
	//double Gtop[1440];	// one line in the file is 60 seconds. The simulation is 86400s => 
				// I must store 86400 / 60 samples = 1440. 

	double Gtop;

	SCA_CTOR(pv_panel):P("P") {}

	void set_attributes();
	
	void initialize();

	void processing();


	private:
	 int counter;

};
