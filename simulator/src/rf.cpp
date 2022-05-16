#include "rf.h"

void rf::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void rf::initialize(){

}

void rf::processing(){

    tmpsoc = SOC.read();
    rf_act_time_in_use_tmp = rf_act_time_in_use.read();


    
    if(tmpsoc > RF_4G_THRESHOLD){
            if(i >= rf_act_time_in_use_tmp && i< rf_act_time_in_use_tmp + RF_4G_TIME_ON){
                P.write(RF_4G_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(RF_IDLE);
                i = (i+1) % PERIOD;
            }
        }

    
    else {
            if(i >= rf_act_time_in_use_tmp && i< rf_act_time_in_use_tmp + RF_3G_TIME_ON){
                P.write(RF_3G_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(RF_IDLE);
                i = (i+1) % PERIOD;
            }
        }


}