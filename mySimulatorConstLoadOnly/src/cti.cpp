#include "cti.h"


void cti::set_attributes(){
}

void cti::initialize(){
}

void cti::processing()
{

 //Compute the total power consumption
 // double total = VREF_CTI*(Isensor1.read()+Isensor2.read()+Imcu.read()+Irf.read());
 double total = VREF_CTI*(Isensor1.read());

 Ptotal.write(total);

}








