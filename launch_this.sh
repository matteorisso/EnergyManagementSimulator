#!/bin/bash

cd simulator

cd src

# delete sensors
ls | grep -P "sensor[0-9]*" | xargs -d"\n" rm
ls | grep -P "converter[0-9]+" | xargs -d"\n" rm -r

# launch python script that creates the system according to settings.json
python3 ./readpattern.py

cd ..

cd work

# build the system
make

# launch the simulation
./run.x

# create matlab script to plot the results
python3 ./read_trace.py

# tuned for my system !
sudo cp ./traces.m /home/anto/Desktop/shared