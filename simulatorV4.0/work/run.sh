#!/bin/bash

make

./run.x

python3 ./read_trace.py

sudo cp ./traces.m /home/anto/Desktop/shared
