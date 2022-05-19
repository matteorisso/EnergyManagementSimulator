// generic includes
#include "systemc-ams.h"
#include "mcu.h"



// sensor 1
#include "sensor1.h"


int sc_main(int argc, char* argv[])
{

    // instantiate signals
   
    sca_tdf::sca_signal<int> Senable1, Sdone1;
    sca_tdf::sca_signal<double> Sdata1;
    

    // instantiate modules  
    sensor1 sensor1("sensor1"); 
    mcu mcu("mcu");
  


    // connect signals to modules
    sensor1.enable(Senable1);
    sensor1.data(Sdata1);
    sensor1.done(Sdone1);
    
    mcu.enableSensor1(Senable1);
    mcu.doneSensor1(Sdone1);
    mcu.readSensor1(Sdata1);
    

    // define simulation file
    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file("trace.dat");

    // the following signals will be traced. Comment any signal you don't want to trace    
    //sca_util::sca_trace( atf, SOC, "SOC" );
    //sca_util::sca_trace( atf, Ptotal, "Ptotal" );
    //sca_util::sca_trace( atf, Pmcu, "Pmcu" );
    //sca_util::sca_trace( atf, Prf, "Prf" );
    //sca_util::sca_trace( atf, Ppv, "Ppv" );
    //sca_util::sca_trace( atf, Psensor1, "Psensor1" );
    //sca_util::sca_trace( atf, Isensor1, "Isensor1" );
    //sca_util::sca_trace( atf, Psensor2, "Psensor2" );
    //sca_util::sca_trace( atf, Isensor2, "Isensor2" );
    //sca_util::sca_trace( atf, Psensor3, "Psensor3" );
    //sca_util::sca_trace( atf, Isensor3, "Isensor3" );
    //sca_util::sca_trace( atf, Psensor4, "Psensor4" );
    //sca_util::sca_trace( atf, Isensor4, "Isensor4" );

    cout<<"The simulation starts!"<<endl;

    sc_start(86400,sc_core::SC_SEC); // Set the simulation length

    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

    sca_util::sca_close_tabular_trace_file( atf );

    return 0;

}
