from jinja2 import Template
import json

json_file = open("./settings.json", "r")
settings = json.load(json_file)

activeSensors = settings["sensors"]

### sensorX.cpp ###

with open('./templates/templateSensorCpp.txt') as templateSensorCpp:
    template = Template(templateSensorCpp.read())
    for sensor in activeSensors:
        sensorName = sensor["name"]
        with open(f'{sensorName}' + '.cpp', 'w') as sensorCpp:
            sensorCpp.write(template.render(sensorName=sensorName))
    templateSensorCpp.close()

### sensorX.h ###
with open('./templates/templateSensorH.txt') as templateSensorH:
    template = Template(templateSensorH.read())
    for sensor in activeSensors:
        sensorName = sensor["name"]
        with open(f'{sensorName}' + '.h', 'w') as sensorH:
            sensorH.write(template.render(sensorName=sensorName))
    templateSensorH.close()

### config.h ###

# define generic params
with open("config.h", "w") as configH:
    configH.write("// generic simulator params\n")
    configH.write(f'#define SIM_STEP {settings["sim_step"]}\n')
    configH.write(f'#define VREF_CTI {settings["vref_cti"]}\n')
    configH.write(f'#define SOC_INIT {settings["soc_init"]}\n')
    configH.write(f'#define SELFDISCH_FACTOR {settings["selfdisch_factor"]}\n')
    configH.write(f'#define PERIOD {settings["period"]}\n')
    configH.close()

# define params for sensors
with open('./templates/templateConfigSensor.txt') as templateConfigSensor:
    template = Template(templateConfigSensor.read())
    sensorsEndAt = 0    # used to know when the MCU can start
    for sensor in activeSensors:
        sensorName = sensor["name"]
        onPower = sensor["power_on"]
        offPower = sensor["power_off"]
        actTime = sensor["activation_time"]
        onTime = sensor["time_on"]
        with open(f'config.h', 'a') as configH:
            configH.write(template.render(sensorName=sensorName, onPower=onPower, offPower=offPower, actTime=actTime, onTime=onTime))
        configH.close()
        possibleNewMax = int(actTime) + int(onTime)
        if possibleNewMax > sensorsEndAt:
            sensorsEndAt = possibleNewMax
        
# define params for MCU
with open('./templates/templateMCU.txt') as templateMCU:
    template = Template(templateMCU.read())
    mcu = settings["mcu"]
    mcu_idle = mcu["mcu_idle"]
    states = mcu["states"]
    mcu_act_time = sensorsEndAt + 1
    with open(f'config.h', 'a') as configH:
        configH.write(template.render(mcu_idle=mcu_idle, mcu_act_time=mcu_act_time, states=states))
    configH.close()
    # THERE MUST BE ONE FOR EACH STATE
    #mcuEndAt1 = int(mcu_act_time) + int(mcu_t1)
    #mcuEndAt2 = int(mcu_act_time) + int(mcu_t2)

# define params for RF
with open('./templates/templateRF.txt') as templateRF:
    template = Template(templateRF.read())
    rf = settings["rf"]
    rf_p1 = rf["rf_p1"]
    rf_p2 = rf["rf_p2"]
    rf_t1 = rf["rf_t1"]
    rf_t2 = rf["rf_t2"]
    rf_idle = rf["rf_idle"]
    # THERE MUST BE ONE FOR EACH STATE
    rf_act_time1 = 1
    rf_act_time2 = 1
    with open(f'config.h', 'a') as configH:
        configH.write(template.render(rf_p1=rf_p1, rf_p2=rf_p2, rf_t1=rf_t1, rf_t2=rf_t2, rf_idle=rf_idle, rf_act_time1=rf_act_time1, rf_act_time2=rf_act_time2))
    configH.close()
templateRF.close()

### mcu.cpp ###

with open('./templates/templateMCUcpp.txt') as templateMCU:
    template = Template(templateMCU.read())
    with open("mcu.cpp", 'w') as w:
        w.write(template.render(states=states))


### converterX.h ###

with open('./templates/templateConverterH.txt') as templateConverterH:
    template = Template(templateConverterH.read())
    sensorID = 0
    for sensor in activeSensors:
        sensorID += 1
        with open(f'converter{sensorID}' + '.h', 'w') as converterH:
            converterH.write(template.render(sensorID=sensorID))
    templateConverterH.close()

### converterX.cpp ###

with open('./templates/templateConverterCpp.txt') as templateConverterCpp:
    template = Template(templateConverterCpp.read())
    sensorID = 0
    for sensor in activeSensors:
        sensorID += 1
        with open(f'converter{sensorID}' + '.cpp', 'w') as converterCpp:
            converterCpp.write(template.render(sensorID=sensorID))
    templateConverterCpp.close()

### main.cpp ###

with open('./templates/templateMainCpp.txt') as templateMainCpp:
    template = Template(templateMainCpp.read())
    with open("main.cpp", "w") as mainCpp:  
        activeSensors = settings["sensors"]
        sensorNames = []
        sensorID = 0
        for sensor in activeSensors:
            sensorID += 1
            sensorNames.append(sensorID) 
        mainCpp.write(template.render(activeSensors=sensorNames))

### cti.h ###

with open('./templates/templateCtiH.txt') as templateCTIH:
    template = Template(templateCTIH.read())
    with open("cti.h", "w") as ctiH:
        activeSensors = settings["sensors"]
        sensorNames = []
        sensorID = 0
        for sensor in activeSensors:
            sensorID += 1
            sensorNames.append(sensorID)         
        ctiH.write(template.render(activeSensors = sensorNames))
        ctiH.close()
templateCTIH.close()

### cti.cpp ###

with open('./templates/templateCtiCpp.txt') as templateCTICpp:
    template = Template(templateCTICpp.read())
    with open("cti.cpp", "w") as ctiCpp:
        activeSensors = settings["sensors"]
        sensorNames = []
        sensorID = 0
        for sensor in activeSensors:
            sensorID += 1
            sensorNames.append(sensorID)         
        ctiCpp.write(template.render(activeSensors = sensorNames))
        ctiCpp.close()
templateCTICpp.close()


### Makefile ###

with open('./templates/Makefile/templateMakefile.txt') as templateMakefile:
    template = Template(templateMakefile.read())
    with open("../work/Makefile", "w") as makefile:
        activeSensors = settings["sensors"]
        sensorNames = []
        sensorID = 0
        for sensor in activeSensors:
            sensorID += 1
            sensorNames.append(sensorID)  
        makefile.write(template.render(activeSensors = sensorNames))
        makefile.close()

### MakeFile dependencies ###

#templateMF = open('./templates/MakefileDeps/templateBaseMakefile.txt')
#fileTBW = open('./myNameIsJeff.txt','w')

# prima scrivo tutto il basefile
with open ('./templates/MakefileDeps/templateBaseMakefileDeps.txt') as templateBaseMakefile:
    template = Template(templateBaseMakefile.read())
    with open("../work/Makefile.deps", "w") as makefile:
        makefile.write(template.render())
    templateBaseMakefile.close()
makefile.close()

with open('./templates/MakefileDeps/templateMakefileSensor.txt') as templateConfigSensor:
    template = Template(templateConfigSensor.read())
    for sensor in activeSensors:
        sensorName = sensor["name"]
        with open("../work/Makefile.deps", 'a') as configH: 
            configH.write(template.render(sensorName=sensorName))
        configH.close()
templateConfigSensor.close()

with open('./templates/MakefileDeps/templateMakefileConverterSensor.txt') as templateConfigSensor:
    template = Template(templateConfigSensor.read())
    converterName = 0
    for sensor in activeSensors:
        converterName += 1
        with open("../work/Makefile.deps", 'a') as configH:
            configH.write(template.render(converterName=converterName))
        configH.close()
templateConfigSensor.close()

with open('./templates/MakefileDeps/templateMakefileMain.txt') as templateMainConfig:
    template = Template(templateMainConfig.read())
    sensorID = 0
    sensorNames = []
    for sensor in activeSensors:
        sensorID += 1
        sensorNames.append(sensorID) 
        print(sensorNames)
    with open("../work/Makefile.deps", 'a') as configH:
        configH.write(template.render(activeSensors=sensorNames))
    configH.close()
templateMainConfig.close()

# controlla il makefile.deps !!
