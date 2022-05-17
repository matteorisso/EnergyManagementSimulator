// generic includes
#include "systemc-ams.h"
#include "mcu.h"
#include "sensor1.h"



int sc_main(int argc, char* argv[])
{

    // instantiate signals

    // sca_tdf::sca_signal<double> Psensor1,Isensor1; 
    // sca_tdf::sca_signal<double> Pmcu, Imcu;


    // instantiate modules
    mcu mcu("mcu");


    // connect signals to modules
    

    mcu.P(Pmcu);

    sensor1.P(Psensor1);


    // define simulation file
    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file("trace.dat");

    // the following signals will be traced. Comment any signal you don't want to trace    
    sca_util::sca_trace( atf, Pmcu, "Pmcu" );
    sca_util::sca_trace( atf, Psensor1, "Psensor1" );    


    cout<<"The simulation starts!"<<endl;

    sc_start(86400,sc_core::SC_SEC); // Set the simulation length

    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

    sca_util::sca_close_tabular_trace_file( atf );

    return 0;

}