#include "cti.h"


void cti::set_attributes(){
}

void cti::initialize(){
}

void cti::processing()
{

 //Compute the total power consumption
 #ifdef ALL_SENSORS
     double totalConsumed = VREF_CTI*(Isensor1.read()+Isensor2.read()+Imcu.read()+Irf.read());
 #endif
 #ifndef ALL_SENSORS
     double totalConsumed = VREF_CTI*(Isensor1.read());
 #endif

 double totalScavengered = VREF_CTI*(Ipv.read());
 double totalRequested = totalConsumed - totalScavengered;

 //if(totalRequested < 0){	// scavengered power is greater than required power, the battery is not needed
 //    Ptotal.write(0);    
 //} else {
     Ptotal.write(totalRequested);			// RIABILITALO
 //}

}








