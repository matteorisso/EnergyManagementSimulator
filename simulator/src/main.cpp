// generic includes
#include "systemc-ams.h"
#include "battery.h"
#include "converter.h"
#include "cti.h"
#include "converter_pv.h"
#include "pv_panel.h"
#include "mcu.h"
#include "converter_mcu.h"
#include "rf.h"
#include "converter_rf.h"


// sensor 1
#include "sensor1.h"
#include "converter1.h"

// sensor 2
#include "sensor2.h"
#include "converter2.h"

// sensor 3
#include "sensor3.h"
#include "converter3.h"

// sensor 4
#include "sensor4.h"
#include "converter4.h"


int sc_main(int argc, char* argv[])
{

    // instantiate signals
    sca_tdf::sca_signal<double> Ibatt, Vbatt, SOC;
    sca_tdf::sca_signal<double> Psensor1,Isensor1; 
    sca_tdf::sca_signal<double> Psensor2,Isensor2; 
    sca_tdf::sca_signal<double> Psensor3,Isensor3; 
    sca_tdf::sca_signal<double> Psensor4,Isensor4; 
    sca_tdf::sca_signal<double> Pmcu, Imcu;
    sca_tdf::sca_signal<double> Prf, Irf;
    sca_tdf::sca_signal<double> Ptotal, Itotal, Ppv, Ipv;
    sca_tdf::sca_signal<int> rf_act_time_in_use;

    sca_tdf::sca_signal<bool> en1;


    // instantiate modules
    battery batt("battery");
    cti cti_bus("bus");
    converter_pv conv_pv("converter_pv");
    pv_panel pv_pan("pv_panel");
    converter converter("converter");    
    sensor1 sensor1("sensor1"); 
    converter1 converter1("converter1"); 
    sensor2 sensor2("sensor2"); 
    converter2 converter2("converter2"); 
    sensor3 sensor3("sensor3"); 
    converter3 converter3("converter3"); 
    sensor4 sensor4("sensor4"); 
    converter4 converter4("converter4"); 
    mcu mcu("mcu");
    converter_mcu converter_mcu("converter_mcu");
    rf rf("rf");
    converter_rf converter_rf("converter_rf");  


    // connect signals to modules
    batt.I(Ibatt);
    batt.V(Vbatt);
    batt.SOC(SOC);

    converter.in(Ptotal);
    converter.in2(Vbatt);
    converter.out(Ibatt);
    
    pv_pan.P(Ppv);
    
    conv_pv.in(Ppv);
    conv_pv.out(Ipv);
    
    sensor1.P(Psensor1);
    sensor1.en(en1);

    converter1.in(Psensor1);
    converter1.out(Isensor1);
    
    sensor2.P(Psensor2);

    converter2.in(Psensor2);
    converter2.out(Isensor2);
    
    sensor3.P(Psensor3);

    converter3.in(Psensor3);
    converter3.out(Isensor3);
    
    sensor4.P(Psensor4);

    converter4.in(Psensor4);
    converter4.out(Isensor4);
    
    mcu.P(Pmcu);
    mcu.SOC(SOC);
    mcu.rf_act_time_in_use(rf_act_time_in_use);
    
    converter_mcu.in(Pmcu);
    converter_mcu.out(Imcu);
    
    rf.P(Prf);
    rf.SOC(SOC);
    rf.rf_act_time_in_use(rf_act_time_in_use);

    converter_rf.in(Prf);
    converter_rf.out(Irf);

    cti_bus.Imcu(Imcu);
    cti_bus.Irf(Irf);
    cti_bus.Ipv(Ipv);
    cti_bus.Ptotal(Ptotal);
    cti_bus.Isensor1(Isensor1);
    cti_bus.Isensor2(Isensor2);
    cti_bus.Isensor3(Isensor3);
    cti_bus.Isensor4(Isensor4);

    // define simulation file
    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file("trace.dat");

    // the following signals will be traced. Comment any signal you don't want to trace    
    sca_util::sca_trace( atf, SOC, "SOC" );
    sca_util::sca_trace( atf, Ptotal, "Ptotal" );
    sca_util::sca_trace( atf, Pmcu, "Pmcu" );
    sca_util::sca_trace( atf, Prf, "Prf" );
    sca_util::sca_trace( atf, Ppv, "Ppv" );
    sca_util::sca_trace( atf, Psensor1, "Psensor1" );
    sca_util::sca_trace( atf, Isensor1, "Isensor1" );
    sca_util::sca_trace( atf, Psensor2, "Psensor2" );
    sca_util::sca_trace( atf, Isensor2, "Isensor2" );
    sca_util::sca_trace( atf, Psensor3, "Psensor3" );
    sca_util::sca_trace( atf, Isensor3, "Isensor3" );
    sca_util::sca_trace( atf, Psensor4, "Psensor4" );
    sca_util::sca_trace( atf, Isensor4, "Isensor4" );

    cout<<"The simulation starts!"<<endl;

    sc_start(86400,sc_core::SC_SEC); // Set the simulation length

    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

    sca_util::sca_close_tabular_trace_file( atf );

    return 0;

}