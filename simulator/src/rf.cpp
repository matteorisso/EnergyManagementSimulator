#include "rf.h"

void rf::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void rf::initialize(){

}

void rf::processing(){

    tmpsoc = SOC.read();
    rf_act_time_in_use_tmp = rf_act_time_in_use.read();


    
    if(tmpsoc > RF_full_speed_THRESHOLD){
            if(i >= rf_act_time_in_use_tmp && i< rf_act_time_in_use_tmp + RF_full_speed_TIME_ON){
                P.write(RF_full_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(RF_IDLE);
                i = (i+1) % PERIOD;
            }
        }

    
    else if(tmpsoc > MCU_medium_speed_THRESHOLD){
            if(i >= rf_act_time_in_use_tmp && i< rf_act_time_in_use_tmp + RF_medium_speed_TIME_ON){
                P.write(RF_medium_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(RF_IDLE);
                i = (i+1) % PERIOD;
            }
        }

    
    else {
            if(i >= rf_act_time_in_use_tmp && i< rf_act_time_in_use_tmp + RF_low_speed_TIME_ON){
                P.write(RF_low_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(RF_IDLE);
                i = (i+1) % PERIOD;
            }
        }


}