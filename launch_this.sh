#!/bin/bash

cd simulator

cd src

python3 ./reader.py

cd ..

cd work

make

./run.x

python3 ./read_trace.py

sudo cp ./traces.m /home/anto/Desktop/shared
