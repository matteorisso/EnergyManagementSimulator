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

// sensor S1
#include "sensor1.h"
#include "converter1.h"

// sensor S2
#include "sensor2.h"
#include "converter2.h"

int sc_main(int argc, char* argv[])
{

    sca_tdf::sca_signal<double> Ibatt, Vbatt, SOC;
    sca_tdf::sca_signal<double> Psensor1, Isensor1;
    sca_tdf::sca_signal<double> Psensor2, Isensor2;
    sca_tdf::sca_signal<double> Pmcu, Imcu;
    sca_tdf::sca_signal<double> Prf, Irf;
    sca_tdf::sca_signal<double> Ptotal, Itotal, Ppv, Ipv;

    battery batt("battery");
    cti cti_bus("bus");
    converter_pv conv_pv("converter_pv");
    pv_panel pv_pan("pv_panel");
    converter converter("converter");
    sensor1 sensor1("sensor1");
    converter1 converter1("converter1");
    sensor2 sensor2("sensor2");
    converter2 converter2("converter2");
    mcu mcu("mcu");
    converter_mcu converter_mcu("converter_mcu");
    rf rf("rf");
    converter_rf converter_rf("converter_rf");


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
    converter1.in(Psensor1);
    converter1.out(Isensor1);
    sensor2.P(Psensor2);
    converter2.in(Psensor2);
    converter2.out(Isensor2);
    mcu.P(Pmcu);
    mcu.SOC(SOC);
    converter_mcu.in(Pmcu);
    converter_mcu.out(Imcu);
    cti_bus.Imcu(Imcu);
    rf.P(Prf);
    rf.SOC(SOC);
    converter_rf.in(Prf);
    converter_rf.out(Irf);
    cti_bus.Irf(Irf);
    cti_bus.Ipv(Ipv);
    cti_bus.Ptotal(Ptotal);
    cti_bus.Isensor1(Isensor1);
    cti_bus.Isensor2(Isensor2);

    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file("trace.dat");

    sca_util::sca_trace( atf, SOC, "SOC" );
    sca_util::sca_trace( atf, Ptotal, "Ptotal" );
    sca_util::sca_trace( atf, Ppv, "Ppv" );

    cout<<"The simulation starts!"<<endl;

    sc_start(86400,sc_core::SC_SEC); // Set the simulation length

    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

    sca_util::sca_close_tabular_trace_file( atf );

    return 0;

}