# MARTe2 DAN demo 

This demo must be run in a CCS version >= 6.1.0.

## Prepare 
Create a folder named Projects and clone MARTe2 Core,  MARTe2 components and the MARTe2 demos:

 `mkdir ~/Projects`
 
 `cd ~/Projects`

 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev`

 `git clone -b develop https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git`
 
 `git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova.git`


Export all the relevant environment variables

 `export TARGET=x86-linux`

 `export MARTe2_DIR=~/Projects/MARTe2-dev`

 `export MARTe2_Components_DIR=~/Projects/MARTe2-components`
 
 `export MDSPLUS_DIR=`

## Compile

Compile MARTe2 and the MARTe2-components.

 `cd ~/Projects/MARTe2-dev`

 `make -f Makefile.gcc`

 `cd ~/Projects/MARTe2-components`

 `make -f Makefile.gcc`

Compile the ADCSimulator component.

 `cd ~/Projects/MARTe2-demos-padova/DataSources/ADCSimulator/`

 `make -f Makefile.gcc`

## Start the IOC

Open a new shell and start the IOC:

 `cd ~/Projects/MARTe2-demos-padova/Configurations`

 `softIoc -d EPICSv3-demo.db`

## Start the DAN backend

Open a new shell and start the DAN backend

 `cd ~/Projects/MARTe2-demos-padova/Configurations`

 `/opt/codac/bin/danApiTool api close all`

 `/opt/codac/bin/danApiTool api init ./DANConfig.xml`

## Start MARTe2

Open a new shell and launch the MARTe2 application

 `cd ~/Projects/MARTe2-demos-padova/Startup`

 `./Main.sh -f ../Configurations/RTApp-DAN.cfg -l RealTimeLoader -m StateMachine:START`

## Monitoring the PVs
 
The following PVs can be monitored:

```
MARTE2-DEMO-APP:HIST-IDLE-CT
MARTE2-DEMO-APP:HIST-RUN-CT
MARTE2-DEMO-APP:STATUS
MARTE2-DEMO-APP:RESET
MARTE2-DEMO-APP:TIME
MARTE2-DEMO-APP:COUNTER
MARTE2-DEMO-APP:ADC0S
MARTE2-DEMO-APP:ADC1S
MARTE2-DEMO-APP:ADC2S
MARTE2-DEMO-APP:ADC3S
```

## Store data

To start storing data into DAN issue the following command:

`caput MARTE2-DEMO-APP:COMMAND 1`

**NOTE**: there is a significant amount of simulated data being stored (2 MHz x 32 bit x 4 channels)

Stop storing with:

`caput MARTE2-DEMO-APP:COMMAND 0`

## Analysing the data

Data is stored in /tmp/data. Use the DAN tools (`h5dump`, `dan-plot`, ...) to inspect the data.

Example: `dan-plot VTA /tmp/data/ICMARTeApp_DANSTREAMER0_DANWriter_int32_111_1600592542072737405.h5 ADC3F`

