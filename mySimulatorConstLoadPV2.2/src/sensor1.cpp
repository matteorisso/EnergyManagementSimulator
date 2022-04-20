#include "sensor1.h"

// uso questo sensore come constant load

void sensor1::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void sensor1::initialize(){
     srand(time(NULL));
}

void sensor1::processing(){

 //if(i==PERIOD){
 //  i = 0;
 //}else {  

   if(i == 0){

      RANDOMIZER = rand();
      RANDOMIZER = RANDOMIZER % 5; 
      RANDOMIZER2 = rand();
      RANDOMIZER2 = RANDOMIZER2 % 4;

      switch(RANDOMIZER){
          case 0 :
	      DUTY_OF_THIS_PERIOD = LOAD_T0;
              break;
          case 1 :  
	      DUTY_OF_THIS_PERIOD = LOAD_T1;
	      break;
          case 2 :  
              DUTY_OF_THIS_PERIOD = LOAD_T2;
	      break;
          case 3 :  
              DUTY_OF_THIS_PERIOD = LOAD_T3;
	      break;
          case 4 :  
              DUTY_OF_THIS_PERIOD = LOAD_T4;
	      break;
	  default:
              DUTY_OF_THIS_PERIOD = LOAD_T0;
      }

      switch(RANDOMIZER2){
          case 0 :
	      LOAD_OF_THIS_PERIOD = LOAD_Pactive1;
              break;
          case 1 :  
	      LOAD_OF_THIS_PERIOD = LOAD_Pactive2;
	      break;
          case 2 :  
              LOAD_OF_THIS_PERIOD = LOAD_Pactive3;
	      break;
          case 3 :  
              LOAD_OF_THIS_PERIOD = LOAD_Pactive4;
	      break;
	  default:
              LOAD_OF_THIS_PERIOD = LOAD_Pactive1;
      }

	cout << "LOAD_OF_THIS_PERIOD " << LOAD_OF_THIS_PERIOD << endl;
        cout << "DUTY_OF_THIS_PERIOD " << RANDOMIZER2 << endl;
      
   }

   if(i<DUTY_OF_THIS_PERIOD){
     P.write(LOAD_OF_THIS_PERIOD);
     i = (i + 1) % PERIOD;
   }else {
     P.write(LOAD_Pidle);
     i = (i + 1) % PERIOD;
   }
 //}


 // prima era un po' sfasato, ora fixato ma forse mi causera' problemi in futuro. Lascio parti commentate.

}
