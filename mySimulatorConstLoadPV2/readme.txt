V2.0

This version includes:

1) Self discharge effect of the battery;
2) Duty cycled Pc (sensor1.cpp is used as constant load, but duty cycled at 50%) with period T = 100s;
3) Scavenger module with values of irradiance. It:
       - is variable, the values of irradiance change in time;
       - is read every period T = 100s in order to keep the load profile and supply profile alligned, as required in the slides. 
	 This assumption can be considered true if we consider the values of irradiance provided by Iukai and we interpolate 
         them with a new X axis;
       - Provides power to the battery.

I kept this version separated wrt the previous one because I might need the previous one in some later modifications.
I will try to comment this version as much as possible.
