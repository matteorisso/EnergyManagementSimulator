#include "cti.h"


void cti::set_attributes(){}

void cti::initialize(){}

void cti::processing(){

//Compute the total power consumption
double totalConsumed = VREF_CTI*(Imcu.read() +  Isensor1.read() +  Isensor2.read() +  Isensor3.read() + Irf.read());

double totalScavengered = VREF_CTI*(Ipv.read());

double totalRequested = totalConsumed - totalScavengered;

Ptotal.write(totalRequested);     // totalRequested >= 0  ?  power_drained_from_battery  :  power_supplied_to_battery

}