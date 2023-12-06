#!../../bin/linux-x86_64/IsttokMarte

#- You may have to change IsttokMarte to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/IsttokMarte.dbd"
IsttokMarte_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/isttok.db","user=bernardo")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=bernardo"
