TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = IsttokMarte
# IsttokMarte.dbd will be created and installed
DBD += IsttokMarte.dbd

# IsttokMarte.dbd will be made up from these files:
IsttokMarte_DBD += base.dbd
IsttokMarte_DBD += PVAServerRegister.dbd

# Include dbd files from all support applications:
#IsttokMarte_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#IsttokMarte_LIBS += xxx

# IsttokMarte_registerRecordDeviceDriver.cpp derives from IsttokMarte.dbd
IsttokMarte_SRCS += IsttokMarte_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
IsttokMarte_SRCS_DEFAULT += IsttokMarteMain.cpp
IsttokMarte_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#IsttokMarte_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link QSRV (pvAccess Server) if available
#ifdef EPICS_QSRV_MAJOR_VERSION
    IsttokMarte_LIBS += qsrv
    IsttokMarte_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    IsttokMarte_DBD += PVAServerRegister.dbd
    IsttokMarte_DBD += qsrv.dbd
#endif

# Finally link to the EPICS Base libraries
IsttokMarte_LIBS += $(EPICS_BASE_IOC_LIBS)
IsttokMarte_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

