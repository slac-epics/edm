TOP = ../../..
ifneq ($(wildcard $(TOP)/config)x,x)
  # New Makefile.Host config file location
  include $(TOP)/config/CONFIG_EXTENSIONS
  EDMTOP = $(TOP)/src/edm
  include $(EDMTOP)/configure/CONFIG_EDM
  include $(TOP)/config/RULES_ARCHS
else
  # Old Makefile.Unix config file location
  EPICS=../../..
  include $(EPICS)/config/CONFIG_EXTENSIONS
  include $(EPICS)/config/RULES_ARCHS
endif

