#include "pv_panel.h"

void pv_panel::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void pv_panel::initialize(){

	top.open("../src/my_one_year_sun_south.txt");		// file with mock measures of irradiance
	
	if(!top) {
	    cout << "couldn't open irradiance file.\n" << endl;
	    exit(-1);	
	}

	i = 0;
	counter = 0;

}

void pv_panel::processing(){


        // hypothesis: sensing of irradiance takes place every PERIOD s, in order for the frame of Pc to be alligned with the frame of Ps.

	if(counter % PERIOD == 0) {
	    top >> Gtop;
	}

	if(Gtop <= 0) {
	    P.write(0);	
	} else {
	    P.write(Gtop / 5.0);	// here I divide by 5.0 just because the mock irradiance vals I created are too high
            // P.write(Gtop);		
	}

	counter ++;

}
