TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = edmTest
# edmTest.dbd will be created and installed
DBD += edmTest.dbd

# edmTest.dbd will be made up from these files:
edmTest_DBD += base.dbd

# edmTest_registerRecordDeviceDriver.cpp derives from edmTest.dbd
edmTest_SRCS += edmTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
edmTest_SRCS_DEFAULT += edmTestMain.cpp

# Finally link to the EPICS Base libraries
edmTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

