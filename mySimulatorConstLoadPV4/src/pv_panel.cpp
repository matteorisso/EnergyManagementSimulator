#include "pv_panel.h"

void pv_panel::set_attributes(){
 P.set_timestep(SIM_STEP, sc_core::SC_SEC);
}

void pv_panel::initialize(){
    // irradiance = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,46.386,84.572,118,182.78,269.29,362.54,462.94,558.09,649.04,730.41,782.68,841.44,894.97,913.59,976.77,947.09,921.17,866.97,796.72,711.22,613.96,497.17,385.95,279.39,175.89,59.373,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	// for(i = 0; i < 144; i++)
	//     irradiance[i] = ((double) i) / 1000;    
	// i = 0;

	// while(getline(file, data)){
        //     sscanf(data.c_str(),"%d,%f",&timestamp, &val);
	//     irradiance[i] = val;
	//     i++;        
	// }
        // i = 0;

	top.open("../src/my_one_year_sun_south.txt");		// se metto solo ./irradianceFile.txt non funziona, per qualche motivo
	
	if(!top) {
	    cout << "couldn't open irradiance file.\n" << endl;
	    exit(-1);	
	}

	i = 0;
	counter = 0;

}

void pv_panel::processing(){
	
	// if(counter % 60 == 0) {
	
	//	if(i < 1440) {			// messo solo perché al momento ho un vett di 144
	//	top >> Gtop[i];
	//	cout << Gtop[i] << "@" <<sc_time_stamp() << " index " << i <<endl;
	//	i++;
	//	}	
	//}

	//P.write(Gtop[i]);	

	//counter ++;

	
	////////P.write(0.4);



	// ipotizzo misure della irradiance fatte ogni 100s <=> stesso periodo dei load

	if(counter % 100 == 0) {
	    top >> Gtop;
	}

	if(Gtop <= 0) {
	    P.write(0);	
	} else {
	    P.write(Gtop / 5.0);
	}

	counter ++;

}
