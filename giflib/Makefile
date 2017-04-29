TOP = ../../..
ifdef EPICS_HOST_ARCH
  include $(TOP)/configure/CONFIG
  EDMTOP = $(TOP)/src/edm
  include $(EDMTOP)/configure/CONFIG_EDM

GIF_LIB ?= /usr/lib
GIF_INC ?= /usr/include

   USR_LIBS += EdmBase cfcaa62e-8199-11d3-a77f-00104b8742df
   USR_LIBS += 114135a4-6f6c-11d3-95bc-00104b8742df

   USR_LIBS_Linux += Xm Xt Xp Xtst X11
   USR_SYS_LIBS_Linux += pthread dl ungif gif

   USR_LIBS_Darwin += Xm Xt Xp Xtst X11
   USR_SYS_LIBS_Darwin += pthread dl ungif gif

   USR_LIBS_solaris += Xm Xt Xmu X11 Xext
   USR_SYS_LIBS_solaris += pthread dl rt ungif gif

   USR_LIBS_hpux11_11_mt = Xm Xt X11 Xext
   USR_SYS_LIBS_hpux11_11_mt = pthread ungif gif

   LIBRARY = cf322683-513e-4570-a44b-7cdd7cae0de5

   INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/edm
   USR_INCLUDES += -I../../util -I../../lib

   USR_INCLUDES += -I$(MOTIF_INC) -I$(X11_INC)
   USR_INCLUDES_solaris += -I$(GIF_INC)
   USR_LDFLAGS_solaris += -L$(GIF_LIB)

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
   

   LIB_SRCS += gif.cc
   LIB_SRCS += reg_libcf322683-513e-4570-a44b-7cdd7cae0de5.cc
   
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

