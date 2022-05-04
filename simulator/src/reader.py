import json
import string

f = json.load(open('settings.json'))
sensorsList = f['sensors']

f = json.load(open('settings.json'))
sensorsList = f['sensors']

### CONFIG.H ###

configFile = open('config.h','w')

sim_step = f['sim_step']
configFile.write("// simulation step\n" + '#define SIM_STEP ' + str(sim_step) + '\n\n')
voltage_bus = f['vref_cti']
configFile.write("// voltage bus\n" + '#define VREF_CTI ' + str(voltage_bus) + '\n\n')
soc_init = f['soc_init']
configFile.write("// initial state of charge battery\n" + '#define SOC_INIT ' + str(soc_init) + '\n\n')
selfdisch_factor = f['selfdisch_factor']
configFile.write("// self discharge factor\n" + '#define SELFDISCH_FACTOR ' + str(selfdisch_factor) + '\n\n')
period = f['period']
configFile.write("// period\n" + '#define PERIOD ' + str(period) + '\n\n')

for sensor in sensorsList:
    sensorname = sensor["name"]
    if sensor["inserted"] == "true":
        sensor_on_power = "#define " + sensorname + "_" + "ON " + sensor["power_on"]
        sensor_off_power = "#define " + sensorname + "_" + "IDLE " + sensor["power_off"]
        sensor_activation_time = "#define " + sensorname + "_" + "ACT_TIME " + sensor["activation_time"]
        sensor_on_time = "#define " + sensorname + "_" + "ON_TIME " + sensor["time_on"]
        configFile.write(f"// params for sensor {sensorname}" "\n" + sensor_on_power + "\n" + sensor_on_time + "\n" + sensor_off_power + "\n" + sensor_activation_time + "\n\n")
    else:
        sensor_on_power = "#define " + sensorname + "_" + "ON 0"
        sensor_off_power = "#define " + sensorname + "_" + "IDLE 0"
        sensor_activation_time = "#define " + sensorname + "_" + "ACT_TIME 0"
        sensor_on_time = "#define " + sensorname + "_" + "ON_TIME 0"
        configFile.write(f"// params for sensor {sensorname}" "\n" + sensor_on_power + "\n" + sensor_on_time + "\n" + sensor_off_power + "\n" + sensor_activation_time + "\n\n")

mcu = f['mcu']
if mcu["inserted"] == "true":
    mcu_p1 = "#define MCU_P1 " + mcu["mcu_p1"]
    mcu_p2 = "#define MCU_P2 " + mcu["mcu_p2"]
    mcu_t1 = "#define MCU_T1 " + mcu["mcu_t1"]
    mcu_t2 = "#define MCU_T2 " + mcu["mcu_t2"]
    mcu_idle = "#define MCU_IDLE " + mcu["mcu_idle"]
    max_end_time_sensors = 0
    for sensor in sensorsList:
        possibleMax = int(sensor["activation_time"]) + int(sensor["time_on"])
        if possibleMax > max_end_time_sensors:
            max_end_time_sensors = possibleMax
    mcu_act_time = "#define MCU_ACT_TIME " + str(max_end_time_sensors)
    configFile.write(f"// params for mcu" "\n" + mcu_p1 + "\n" + mcu_p2 + "\n" + mcu_t1 + "\n" + mcu_t2 + "\n" + mcu_idle + "\n" + mcu_act_time + "\n\n")

rf = f['rf']
if rf["inserted"] == "true":
    rf_p1 = "#define RF_P1 " + rf["rf_p1"]
    rf_p2 = "#define RF_P2 " + rf["rf_p2"]
    rf_t1 = "#define RF_T1 " + rf["rf_t1"]
    rf_t2 = "#define RF_T2 " + rf["rf_t2"]
    rf_idle = "#define RF_IDLE " + rf["rf_idle"]
    mcu_end_time1 = int(max_end_time_sensors) + int(mcu["mcu_t1"])
    mcu_end_time2 = int(max_end_time_sensors) + int(mcu["mcu_t2"])
    rf_act_time1 = "#define RF_ACT_TIME1 " + str(mcu_end_time1)
    rf_act_time2 = "#define RF_ACT_TIME2 " + str(mcu_end_time2)
    configFile.write(f"// params for rf" "\n" + rf_p1 + "\n" + rf_p2 + "\n" + rf_t1 + "\n" + rf_t2 + "\n" + rf_idle + "\n" + rf_act_time1 + "\n" + rf_act_time2 + "\n\n")

configFile.close()

### MAIN.CPP ###

mainFile = open('main.cpp','w')

mainFile.write('#include "systemc-ams.h"\n#include "battery.h"\n#include "converter.h"\n#include "cti.h"\n#include "converter_pv.h"\n#include "pv_panel.h"')

if(mcu["inserted"] == "true"):
    mainFile.write("\n#include \"mcu.h\"\n")
    mainFile.write("#include \"converter_mcu.h\"\n")
if(rf["inserted"] == "true"):
    mainFile.write("#include \"rf.h\"\n")
    mainFile.write("#include \"converter_rf.h\"\n")
mainFile.write("\n")
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        mainFile.write("// sensor " + sensorname + "\n")
        mainFile.write("#include \"sensor" + sensorname[1:] + ".h\"\n")
        mainFile.write("#include \"converter" + sensorname[1:] + ".h\"\n\n")

mainFile.write("int sc_main(int argc, char* argv[])\n{\n\n")
mainFile.write("    sca_tdf::sca_signal<double> Ibatt, Vbatt, SOC;")
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        mainFile.write("\n    sca_tdf::sca_signal<double> Psensor" + sensorname[1:] + ", " + "Isensor" + sensorname[1:] + ";")
if(mcu["inserted"] == "true"):
    mainFile.write("\n    sca_tdf::sca_signal<double> Pmcu, Imcu;\n")
if(rf["inserted"] == "true"):
    mainFile.write("    sca_tdf::sca_signal<double> Prf, Irf;\n")
mainFile.write("    sca_tdf::sca_signal<double> Ptotal, Itotal, Ppv, Ipv;\n\n")
mainFile.write('    battery batt("battery");\n    cti cti_bus("bus");\n    converter_pv conv_pv("converter_pv");\n    pv_panel pv_pan("pv_panel");\n    converter converter("converter");\n')
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        mainFile.write(f'    sensor{sensorname[1:]} ' + f'sensor{sensorname[1:]}("sensor{sensorname[1:]}");\n')
        mainFile.write(f'    converter{sensorname[1:]} ' + f'converter{sensorname[1:]}("converter{sensorname[1:]}");\n')
if(mcu["inserted"] == "true"):
    mainFile.write('    mcu mcu("mcu");\n')
    mainFile.write('    converter_mcu converter_mcu("converter_mcu");\n')
if(rf["inserted"] == "true"):
    mainFile.write('    rf rf("rf");\n')
    mainFile.write('    converter_rf converter_rf("converter_rf");\n')

mainFile.write("\n\n    batt.I(Ibatt);\n    batt.V(Vbatt);\n    batt.SOC(SOC);\n    converter.in(Ptotal);\n    converter.in2(Vbatt);\n    converter.out(Ibatt);\n    pv_pan.P(Ppv);\n    conv_pv.in(Ppv);\n    conv_pv.out(Ipv);\n")
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        sensornumber = sensorname[1:]
        mainFile.write(f'    sensor{sensornumber}.P(Psensor{sensornumber});\n')
        mainFile.write(f'    converter{sensornumber}.in(Psensor{sensornumber});\n')
        mainFile.write(f'    converter{sensornumber}.out(Isensor{sensornumber});\n')
if(mcu["inserted"] == "true"):
    mainFile.write('    mcu.P(Pmcu);\n    mcu.SOC(SOC);\n')
    mainFile.write('    converter_mcu.in(Pmcu);\n    converter_mcu.out(Imcu);\n')
    mainFile.write('    cti_bus.Imcu(Imcu);\n')
if(rf["inserted"] == "true"):
    mainFile.write('    rf.P(Prf);\n    rf.SOC(SOC);\n')
    mainFile.write('    converter_rf.in(Prf);\n    converter_rf.out(Irf);\n')
    mainFile.write('    cti_bus.Irf(Irf);\n')
mainFile.write('    cti_bus.Ipv(Ipv);\n    cti_bus.Ptotal(Ptotal);\n')
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        sensornumber = sensorname[1:]
        mainFile.write(f'    cti_bus.Isensor{sensornumber}(Isensor{sensornumber});\n')

mainFile.write("\n    sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file(\"trace.dat\");\n")

plottedSignals = f["plottedSignals"]
for signal in plottedSignals:
    if signal["inserted"] == "true":
        mainFile.write(f"\n    sca_util::sca_trace( atf, {signal['name']}, \"{signal['name']}\" );")

if f["plotAllSensors"]== "true":
    for sensor in sensorsList:
        if sensor["inserted"] == "true":
            sensorname = sensor["name"]
            sensornumber = sensorname[1:]
            mainFile.write(f"\n    sca_util::sca_trace( atf, Isensor{sensornumber}, \"Isensor{sensornumber}\" );")
            mainFile.write(f"\n    sca_util::sca_trace( atf, Psensor{sensornumber}, \"Psensor{sensornumber}\" );")

mainFile.write('\n\n    cout<<"The simulation starts!"<<endl;\n\n    sc_start(86400,sc_core::SC_SEC); // Set the simulation length\n\n    cout<<"The simulation ends @"<<sc_time_stamp()<<endl;\n\n    sca_util::sca_close_tabular_trace_file( atf );\n\n    return 0;\n\n}')

mainFile.close()

### CTI.CPP ###

cticpp = open("cti.cpp", "w")

cticpp.write('#include "cti.h"\n\n\nvoid cti::set_attributes(){}\n\nvoid cti::initialize(){}\n\nvoid cti::processing(){\n\n//Compute the total power consumption\n')

stringToBeAttached = ""
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        sensornumber = sensorname[1:]
        stringToBeAttached += f"+ Isensor{sensornumber}.read()"

cticpp.write('double totalConsumed = VREF_CTI*(Imcu.read() + Irf.read() ' + stringToBeAttached + ');\n\n')

cticpp.write('double totalScavengered = VREF_CTI*(Ipv.read());\n\n')

cticpp.write('double totalRequested = totalConsumed - totalScavengered;\n\n')

cticpp.write('Ptotal.write(totalRequested);     // totalRequested >= 0  ?  power_drained_from_battery  :  power_supplied_to_battery\n\n}')

cticpp.close()

### CTI.H ###

ctih = open("cti.h", "w")

ctih.write('#include "systemc-ams.h"\n#include "config.h"\n\nSCA_TDF_MODULE (cti)\n{\n\n')

stringToBeAttached = ""
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        sensornumber = sensorname[1:]
        stringToBeAttached += f", Isensor{sensornumber}"

ctih.write('sca_tdf::sca_in<double> Imcu, Irf' + stringToBeAttached + ';\n')

ctih.write('sca_tdf::sca_out<double>  Ptotal; // Compute the total power consumption for battery module\n\nsca_tdf::sca_in<double> Ipv;')

stringToBeAttached = '\n\nSCA_CTOR(cti): Ptotal("Ptotal")'
for sensor in sensorsList:
    if sensor["inserted"] == "true":
        sensorname = sensor["name"]
        sensornumber = sensorname[1:]
        stringToBeAttached += f', Isensor{sensornumber}("Isensor{sensornumber}")'
stringToBeAttached += ', Imcu("Imcu"), Irf("Irf"), Ipv("Ipv"){};\n\n'

ctih.write(stringToBeAttached)

ctih.write('\nvoid set_attributes();\n\nvoid initialize();\n\nvoid processing();\n\n};')

ctih.close()


### altri file non sembra vadano toccati ###