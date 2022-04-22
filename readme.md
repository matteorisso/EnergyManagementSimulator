###IoT SYSTEM SIMULATOR

## Features

The current version of the simulator provides:
 1) a photovoltaic module; <br />
 2) inefficiency factors for all converters; <br />
 3) self-discharge factor for the battery; <br />
 4) possibility to switch between a system with a single constant load and a more realistic system with MCU, RF, SENSOR1, SENSOR2, PV; <br />
 5) possibility to set the Pidle of any device to a value greater than 0; <br />
 6) possibility to change the schedule of the activation of the sensors (sequential or parallel); <br />
 7) estimated Ps, with mock measures taken from an input file. <br />

## Usage Info

I suggest to give a look at the configuration file, first.
Here, you can find 3 user-manageable parameters: {ALL_SENSORS, PARALLEL, DETERMINISTIC_LOAD}.

The behavior of the simulator changes according to the presence of the definition of the parameters forementioned.
Denoting with '1' the presence of the definition, with '0' its absence and with 'X' its uninfluence:

ALL_SENSORS    PARALLEL    DETERMINISTIC_LOAD    |    BEHAVIOR
    0             X                  0          ==>  system has SENSOR1, PV, battery and the duty cycle of SENSOR1 is a random one among those defined in config.h, for each frame <br />
    0             X                  1          ==>  system has SENSOR1, PV, battery and the duty cycle of SENSOR1 is set to LOAD_Tdefault by default <br />
    1	            0                  X          ==>  system has SENSOR1, SENSOR2, MCU, RF, PV, battery and the two sensors are activated in sequence <br />
    1             1                  X          ==>  system has SENSOR1, SENSOR2, MCU, RF, PV, battery and the two sensors are activated in parallel <br />

The output of the simulation will be in the /work folder. In order to have vectors that can be plotted on Matlab, you can run the command in the /work folder:
  python ./read_trace.py  (or python3).

## Latest Update

Updated on 22/04/2022.

## Notes

P.S. the file /work/run.sh is a bash script that compiles the program, runs the simulation, parses the output to Matlab vectors but it is tuned to MY machine. You can comment the last line to use it.
