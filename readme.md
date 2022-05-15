# IoT System Simulator

## Features

The current version of the simulator provides:
 1) a photovoltaic module; <br />
 2) inefficiency factors for all converters; <br />
 3) self-discharge factor for the battery; <br />
 4) possibility to set the Pidle of any device to a value greater than 0; <br />
 5) possibility to change the schedule of the activation of the sensors (sequential or parallel); <br />
 6) estimated Ps, with mock measures taken from an input file. <br />
 7) possibility to insert a variable number of sensors ```diff - NEW! ```

## Usage Info

This release allows you to insert multiple sensors simply by editing the "src/settings.json" file. You can set them in parallel/sequence modifying their activation time. <br />
With respect to the previous version this release offers scalability in terms of the number of sensors that can be inserted, thanks to the usage of Jinja Python library. <br />
In order to play with the simulator you must have Python3 interpreter and the Jinja library. <br />
Once you modify the settings.json you can move two levels of folders upwards with respect to the src folder and launch the bash script called "launch_this.sh". It will automagically:
1) generate source files according to "src/settings.json"; <br />
2) compile the system; <br />
3) run the simulation; <br />
4) generate a Matlab file containing traces of the signals that you wish to plot.
## Latest Update

Updated on 15/05/2022.

## Notes

1) The "launch_this.sh" bash script contains a command that is tuned to my machine, you can delete it; <br />
2) I suggest to call the sensors "sensorX" in the "settings.json" file. I will relax this constraint in a next release. 