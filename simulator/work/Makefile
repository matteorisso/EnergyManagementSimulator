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
SRCS =  ../src/battery_voc.cpp ../src/battery.cpp\
	../src/battery_char.cpp ../src/battery_fre.cpp \
    ../src/sensor1.cpp \
    ../src/sensor2.cpp \
    ../src/sensor3.cpp \
    ../src/pv_panel.cpp \
	../src/mcu.cpp ../src/rf.cpp \
	../src/cti.cpp \
	../src/converter.cpp ../src/converter1.cpp ../src/converter2.cpp ../src/converter3.cpp  \
	../src/converter_mcu.cpp ../src/converter_rf.cpp ../src/converter_pv.cpp \
	../src/main.cpp

OBJS = $(SRCS:../src/%.cpp=%.o)

include Makefile.defs