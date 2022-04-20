#include "systemc-ams.h"
#include "battery_char.h"
#include "battery_voc.h"
#include "battery_fre.h"

// Battery model: modified the battery_voc.cpp to get different battery voc and capacity relationships;
//                                                    different battery voc and internal resistance relationships.

SC_MODULE(battery){

	// Interface and internal components declaration
	sca_tdf::sca_in<double> I; // Battery current
	sca_tdf::sca_out<double> V; // Battery voltage
	sca_tdf::sca_out<double> SOC; //Battery SOC

	// Connecting signals
	sca_tdf::sca_signal<double> Voc,Rs,deltaf,deltac;

	// Instantiation of battery componenets
	battery_voc* voc;
	battery_char* batt;
	battery_fre* freq;

	SC_CTOR(battery): I("I"),V("V"),SOC("SOC")
	{
		voc = new battery_voc("voc");
		batt = new battery_char("batt");
   		freq = new battery_fre("freq");

   		 voc->in(I);
		 voc->in2(deltac);
		 voc->in3(deltaf);
		 voc->out(Voc);
   		 voc->out2(Rs);
		 voc->out3(SOC);
	
   		 batt->in(Rs);
   		 batt->in2(I);
   		 batt->in3(Voc);
  		 batt->out(V);

	         freq->in(I);
		 freq->in2(SOC);
		 freq->out(deltac);
		 freq->out2(deltaf);
	}

};








