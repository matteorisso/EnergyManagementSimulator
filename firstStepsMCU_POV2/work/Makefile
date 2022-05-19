.SUFFIXES:
.SUFFIXES: .cc .cpp .o .x

CC = g++
OPT =
DEBUG = 
OTHER = -Wall -O3

# Use this CFLAGS definition if you need to see debug output
# CFLAGS = -c -g -Wno-deprecated -Wall -D_DEBUG
CFLAGS = -c -g -Wno-deprecated 

MODULE = run
SRCS =  ../src/sensor1.cpp ../src/mcu.cpp ../src/main.cpp ../src/config.h

OBJS = $(SRCS:../src/%.cpp=%.o)

include Makefile.defs
