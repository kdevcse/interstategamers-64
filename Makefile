# 	NuSYSTEM samples nu2 Makefile
N64KITDIR   = c:/nintendo/n64kit
 
TARGET  	= first
CODEFILES   = main.c graphic.c helper.c
OBJECTFILES = assets/includes/stages.o

include $(ROOT)/usr/include/make/PRdefs
 
DEBUGSYM	= -g
OPTIMIZER   = -O0
 
NUSYSDIR	= $(N64KITDIR)/nusys
NUSYSINC	= $(NUSYSDIR)/include
NUSYSLIB	= $(NUSYSDIR)/lib
 
NUOBJ   	= $(NUSYSLIB)/nusys.o
CODEOBJECTS = $(CODEFILES:.c=.o) $(NUOBJ) $(OBJECTFILES)
 
CUSTFLAGS   =
LCINCS  	= -I$(NUSYSINC)
LCOPTS  	= -G 0 $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS 	= -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) \
              -lnusys_d -lgultra_d -L$(GCCDIR)/mipse/lib -lkmc
 
CODESEGMENT = codesegment.o
OBJECTS 	= $(CODESEGMENT)
 
SYMBOL  = $(TARGET).out
ROM 	= $(TARGET).n64
 
default:$(ROM)
 
$(CODESEGMENT): $(CODEOBJECTS)
    	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)
 
$(ROM) :  $(OBJECTS)
    	$(MAKEROM) spec -I$(NUSYSINC) -r $(ROM) -e $(SYMBOL)
		makemask $(ROM)
 
include $(ROOT)/usr/include/make/commonrules
