#include "cti.h"


void cti::set_attributes(){
}

void cti::initialize(){
}

void cti::processing()
{

 //Compute the total power consumption
 // double total = VREF_CTI*(Isensor1.read()+Isensor2.read()+Imcu.read()+Irf.read());
 double totalConsumed = VREF_CTI*(Isensor1.read());
 double totalScavengered = VREF_CTI*(Ipv.read());
 double totalRequested = totalConsumed - totalScavengered;

 if(totalRequested < 0){	// scavengered power is greater than required power, the battery is not needed
     Ptotal.write(0);    
 } else {
     Ptotal.write(totalRequested);
 }

}








