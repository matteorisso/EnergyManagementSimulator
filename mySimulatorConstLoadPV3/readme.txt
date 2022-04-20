V3.0

This version includes, in the /src folder:

1) Self discharge effect of the battery;
2) Pc composed as the sum of RF + MCU + SENSOR1 + SENSOR2;
3) Scavenger module with values of irradiance. It:
       - is variable, the values of irradiance change in time;
       - is read every period T = 100s in order to keep the load profile and supply profile alligned, as required in the slides. 
	 This assumption can be considered true if we consider the values of irradiance provided by Iukai and we interpolate 
         them with a new X axis;
       - Provides power to the battery.

This version includes, in the /work folder:

1) A python script called "read_trace.py" that can be launched with the following command in the /work folder: 
          $ python read_trace.py 
                   or
          $ python3 read_trace.py

   It generates the file "traces.m" that is a file containing vectors to be plotted in matlab.



I kept this version separated wrt the previous one because I might need the previous one in some later modifications.
I will try to comment this version as much as possible.


