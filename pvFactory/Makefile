TOP = ..
ifdef EPICS_HOST_ARCH
  include $(TOP)/configure/CONFIG
  EDMTOP = $(TOP)
  include $(EDMTOP)/configure/CONFIG_EDM

   USR_LIBS_solaris += Xm Xt X11 Xmu
   USR_SYS_LIBS_solaris = pthread dl

   USR_LIBS_hpux11_11_mt = cfcaa62e-8199-11d3-a77f-00104b8742df pthread

   USR_INCLUDES += -I$(MOTIF_INC) -I$(X11_INC)


   Xm_DIR =   $(MOTIF_LIB)
   Xt_DIR =   $(X11_LIB)
   X11_DIR =  $(X11_LIB)
   Xmu_DIR =  $(X11_LIB)
   Xtst_DIR = $(X11_LIB)
  
   # ELW added following two lines for
   # Red Hat 7.0 LINUX
   Xp_DIR =   $(X11_LIB)
   Xext_DIR = $(X11_LIB)
   #
   XlibGUI_DIR = $(X11_LIB)
   XmStatic_DIR = $(MOTIF_LIB)
   XmStatXt_DIR = $(MOTIF_LIB)
   HCLXt_DIR = $(X11_LIB)
   Xlib_DIR = $(X11_LIB)
   HCLXmu_DIR = $(X11_LIB)
   
  
   LIBRARY = PV

   USR_LIBS += cfcaa62e-8199-11d3-a77f-00104b8742df
   USR_LIBS += 114135a4-6f6c-11d3-95bc-00104b8742df

   PV_LIBS_DEFAULT += ca Com
   PV_LIBS_Darwin += -nil-

   INSTALL_INCLUDE = $(INSTALL_LOCATION)/include
   USR_INCLUDES += -I$(TOP)/util -I$(TOP)/edmlib

   LIB_SRCS   += textupdate.cc
   LIB_SRCS   += SciPlot.cc
   LIB_SRCS   += strip_data.cc
   LIB_SRCS   += strip.cc
   LIB_SRCS   += regTextupdate.cc
   LIB_SRCS   += byte.cc
   LIB_SRCS   += pv_register.cc
   
  include $(TOP)/configure/RULES
   
else
   ifneq ($(wildcard $(TOP)/config)x,x)
     # New Makefile.Host config file location
     include $(TOP)/config/CONFIG_EXTENSIONS
     include $(TOP)/config/RULES_ARCHS
   else
     # Old Makefile.Unix config file location
     EPICS=../../..
     include $(EPICS)/config/CONFIG_EXTENSIONS
     include $(EPICS)/config/RULES_ARCHS
   endif
endif

