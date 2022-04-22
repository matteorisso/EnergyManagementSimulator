#include "systemc-ams.h"
#include "config.h"


SCA_TDF_MODULE (cti)
{
  #ifdef ALL_SENSORS
  sca_tdf::sca_in<double> Isensor1, Isensor2, Imcu, Irf;
  #endif
  #ifndef ALL_SENSORS
  sca_tdf::sca_in<double> Isensor1;
  #endif

  sca_tdf::sca_out<double>  Ptotal; // Compute the total power consumption for battery module 
  
  sca_tdf::sca_in<double> Ipv;
  
  #ifdef ALL_SENSORS
      SCA_CTOR(cti): Ptotal("Ptotal"),Isensor1("Isensor1"),Isensor2("Isensor2"),Imcu("Imcu"),Irf("Irf"), Ipv("Ipv"){}
  #endif
  #ifndef ALL_SENSORS
      SCA_CTOR(cti): Ptotal("Ptotal"),Isensor1("Isensor1"),Ipv("Ipv"){}
  #endif


  void set_attributes();

  void initialize();

  void processing();


};
