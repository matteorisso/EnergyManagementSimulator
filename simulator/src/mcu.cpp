#include "mcu.h"

void mcu::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void mcu::initialize(){

}

void mcu::processing(){

    tmpsoc = SOC.read();


    
    if(tmpsoc > MCU_full_speed_THRESHOLD){
            if(i >= MCU_ACT_TIME && i< MCU_ACT_TIME + MCU_full_speed_TIME_ON){
                P.write(MCU_full_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(MCU_IDLE);
                i = (i+1) % PERIOD;
            }
            rf_act_time_in_use_tmp = MCU_ACT_TIME + MCU_full_speed_TIME_ON +1;
        }

    
    else if(tmpsoc > MCU_medium_speed_THRESHOLD){
            if(i >= MCU_ACT_TIME && i< MCU_ACT_TIME + MCU_medium_speed_TIME_ON){
                P.write(MCU_medium_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(MCU_IDLE);
                i = (i+1) % PERIOD;
            }
            rf_act_time_in_use_tmp = MCU_ACT_TIME + MCU_medium_speed_TIME_ON +1;
        }

    
    else {
            if(i >= MCU_ACT_TIME && i< MCU_ACT_TIME + MCU_low_speed_TIME_ON){
                P.write(MCU_low_speed_POWER_ON);
                i = (i+1) % PERIOD;
            }else{
                P.write(MCU_IDLE);
                i = (i+1) % PERIOD;
            }
            rf_act_time_in_use_tmp = MCU_ACT_TIME + MCU_low_speed_TIME_ON +1;
        }


    rf_act_time_in_use.write(rf_act_time_in_use_tmp);

}