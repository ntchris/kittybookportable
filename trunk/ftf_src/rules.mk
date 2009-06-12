
BASE_DIR := $(FTF_DIR)

BASE_SRC := $(BASE_DIR)/ftapi.o    \
            $(BASE_DIR)/ftcalc.o   \
            $(BASE_DIR)/ftdbgmem.o \
            $(BASE_DIR)/ftgloadr.o \
            $(BASE_DIR)/ftnames.o  \
            $(BASE_DIR)/ftobjs.o   \
            $(BASE_DIR)/ftoutln.o  \
            $(BASE_DIR)/ftrfork.o  \
            $(BASE_DIR)/ftstream.o \
            $(BASE_DIR)/fttrigon.o \
            $(BASE_DIR)/ftutil.o

# Base layer `extensions' sources
#
# An extension is added to the library file (.a or .lib) as a separate
# object.  It will then be linked to the final executable only if one of its
# symbols is used by the application.
#
BASE_EXT_SRC := $(BASE_DIR)/ftbitmap.o \
                $(BASE_DIR)/ftbbox.o   \
                $(BASE_DIR)/ftbdf.o    \
                $(BASE_DIR)/ftglyph.o  \
                $(BASE_DIR)/ftmm.o     \
                $(BASE_DIR)/ftotval.o  \
                $(BASE_DIR)/ftpfr.o    \
                $(BASE_DIR)/ftstroke.o \
                $(BASE_DIR)/ftsynth.o  \
                $(BASE_DIR)/fttype1.o  \
                $(BASE_DIR)/ftwinfnt.o \
                $(BASE_DIR)/ftxf86.o
                
                
                

    
                