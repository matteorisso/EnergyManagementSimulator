#include "systemc-ams.h"
#include "config.h"


SCA_TDF_MODULE (cti)
{
  
  // sca_tdf::sca_in<double> Isensor1, Isensor2, Imcu, Irf;
  sca_tdf::sca_in<double> Isensor1;
  sca_tdf::sca_out<double>  Ptotal; // Compute the total power consumption for battery module 
  
  sca_tdf::sca_in<double> Ipv;
  
  // SCA_CTOR(cti): Ptotal("Ptotal"),Isensor1("Isensor1"),Isensor2("Isensor2"),Imcu("Imcu"),Irf("Irf"){}
  SCA_CTOR(cti): Ptotal("Ptotal"),Isensor1("Isensor1"),Ipv("Ipv"){}



  void set_attributes();

  void initialize();

  void processing();


};
