// Nella primissima versione uso solo un load, utilizzando sensor1 come load.

/**
*
*	BATTERY			PV_PANEL
*	  |  |                    |  |
*	  v  v                    v  v
*	CONVERTER	      CONVERTER_PV
*         |  |                    |  | 
*	  v  v			  v  v
*	[ B                 U                S ]
*				  |   |
*	  		 	  v   v
*			       CONVERTER1
*				  |   |
*				  v   v
*			      CONSTANT LOAD
*			   (sensor1, for now)
**/

#include "systemc-ams.h"
#include "battery.h"
#include "cti.h"
#include "sensor1.h"
// #include "sensor2.h"
#include "converter.h"
#include "converter1.h"
// #include "converter2.h"
// #include "converter_mcu.h"
// #include "converter_rf.h"
// #include "mcu.h"
// #include "rf.h"
#include "converter_pv.h"
#include "pv_panel.h"

int sc_main(int argc, char* argv[])
{

  // Define the signals for mapping the different modules
  sca_tdf::sca_signal<double> Ibatt, Vbatt, SOC;
  sca_tdf::sca_signal<double> Psensor1, Psensor2, Pmcu, Prf, Ptotal;
  sca_tdf::sca_signal<double> Isensor1, Isensor2, Imcu, Irf;
  
  sca_tdf::sca_signal<double> Ppv, Ipv;

  // Instantiate Objects
  battery batt("battery");
  sensor1 sensor1("sensor1"); 
  // sensor2 sensor2("sensor2");
  // mcu mcu("mcu");
  // rf rf("rf");
  converter converter("converter");
  converter1 converter1("converter1");
  // converter2 converter2("converter2"); //If the different sensors use the same converter, can use one converter module to instantiate two different instances. The current version assumes all the components have their own converter.
  // converter_mcu converter_mcu("converter_mcu");
  // converter_rf converter_rf("converter_rf");
  cti cti_bus("bus");

  converter_pv conv_pv("converter_pv"); 
  pv_panel pv_pan("pv_panel");

  // Mapping the ports
  batt.I(Ibatt);
  batt.V(Vbatt);
  batt.SOC(SOC);

  converter.in(Ptotal);
  converter.in2(Vbatt);
  converter.out(Ibatt);
  
  sensor1.P(Psensor1);

  converter1.in(Psensor1);
  converter1.out(Isensor1);

  pv_pan.P(Ppv);

  conv_pv.in(Ppv);
  conv_pv.out(Ipv);

  // sensor2.P(Psensor2);

  // converter2.in(Psensor2);
  // converter2.out(Isensor2);

  // mcu.P(Pmcu);
  // mcu.SOC(SOC);
  
  // converter_mcu.in(Pmcu);
  // converter_mcu.out(Imcu);

  // rf.P(Prf);
  // rf.SOC(SOC);
  
  // converter_rf.in(Prf);
  // converter_rf.out(Irf);

  cti_bus.Isensor1(Isensor1); 
  // cti_bus.Isensor2(Isensor2); 
  // cti_bus.Imcu(Imcu); 
  // cti_bus.Irf(Irf); 
  cti_bus.Ptotal(Ptotal);

  cti_bus.Ipv(Ipv);


  // Define the trace file for monitoring the signals profiles
  sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file( "trace.dat" );
  atf->set_mode(sca_decimation(10)); // Define the sampling rate

  // Select the signals want to be tracked 
  sca_util::sca_trace( atf, SOC, "SOC" );
  //sca_util::sca_trace( atf, Ibatt, "Ibatt" );
  //sca_util::sca_trace( atf, Vbatt, "Vbatt" );
  sca_util::sca_trace( atf, Ptotal, "Ptotal" );
  sca_util::sca_trace( atf, Psensor1, "Psensor1" );
  //sca_util::sca_trace( atf, Isensor1, "Isensor1" );
  // sca_util::sca_trace( atf, Psensor2, "Psensor2" );
  //sca_util::sca_trace( atf, Isensor2, "Isensor2" );
  // sca_util::sca_trace( atf, Pmcu, "Pmcu" );
  //sca_util::sca_trace( atf, Imcu, "Imcu" );
  // sca_util::sca_trace( atf, Prf, "Prf" );
  //sca_util::sca_trace( atf, Irf, "Irf" );

  sca_util::sca_trace( atf, Ppv, "Ppv" );
  sca_util::sca_trace( atf, Ipv, "Ipv" );

  cout<<"The simulation starts!"<<endl;

  sc_start(86400,sc_core::SC_SEC); // Set the simulation length

  cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

  sca_util::sca_close_tabular_trace_file( atf );

  return 0;

}
