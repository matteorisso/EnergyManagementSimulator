// generic includes
#include "systemc-ams.h"
#include "cti.h"
#include "mcu.h"

// sensor 1
#include "sensor1.h"


int sc_main(int argc, char* argv[])
{

    // instantiate signals

    // signals CTI <-> sensor1
    sca_tdf::sca_signal<int> CTItoS1_en, CTItoS1_done;
    sca_tdf::sca_signal<double> CTItoS1_data;

    // signals CTI <-> mcu
    sca_tdf::sca_signal<int> CTItoMCU_enS1, CTItoMCU_doneS1;    
    sca_tdf::sca_signal<double> CTItoMCU_dataS1;
   


    // instantiate modules
    cti cti_bus("bus");   
    sensor1 sensor1("sensor1"); 
    mcu mcu("mcu");  

    // connect signals to modules
    sensor1.enable(CTItoS1_en);
    sensor1.data(CTItoS1_data);
    sensor1.done(CTItoS1_done);
    
    cti_bus.doneSensor1_S1(CTItoS1_done);
    cti_bus.enableSensor1_S1(CTItoS1_en);
    cti_bus.dataSensor1_S1(CTItoS1_data);

    cti_bus.doneSensor1_MCU(CTItoMCU_doneS1);
    cti_bus.enableSensor1_MCU(CTItoMCU_enS1);
    cti_bus.dataSensor1_MCU(CTItoMCU_dataS1);
	
    mcu.enableSensor1(CTItoMCU_enS1);
    mcu.doneSensor1(CTItoMCU_doneS1);
    mcu.dataSensor1(CTItoMCU_dataS1);


    // define simulation file
    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file("trace.dat");

    // the following signals will be traced. Comment any signal you don't want to trace    
    /*
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
    */

    cout<<"The simulation starts!"<<endl;

    sc_start(86400,sc_core::SC_SEC); // Set the simulation length

    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;

    sca_util::sca_close_tabular_trace_file( atf );

    return 0;

}
