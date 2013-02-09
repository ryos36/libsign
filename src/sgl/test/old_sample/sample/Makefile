#
# this is for gcc
#
ifeq ($(PLATFORM), ARMADILLO)
PREFIX=arm-linux-
#PREFIX=arm-vfp-linux-gnu-
#CFLAGS += -mfpu=vfp -mfloat-abi=softfp
endif

TOP=../..

include $(TOP)/mk/common.mk

#CFLAGS += --input-charset=cp932

#
# Windows or Other OS ?
#

ifeq ($(OS), Windows_NT)
	TARGET=sgl++sample.exe
	SOURCE_DEVICE_CPP=OpenGLDevice.cpp
else
	TARGET=sgl++sample
	ifeq ($(PLATFORM), ARMADILLO)
		SOURCE_DEVICE_CPP=Armadillo500FXLCDDevice.cpp
		CFLAGS += -D__ARMADILLO500FX__
	else
		SOURCE_DEVICE_CPP=VesaVgaDevice.cpp
	endif
endif

SOURCES_CPP = \
	$(SOURCE_DEVICE_CPP) \
	sgl_init.cpp \
	sample.cpp \
	$(NULL)

SOURCES_C = 

LIBS=-lsgl++
ifeq ($(OS), Windows_NT)
LIBS+=-lglut32 -lglu32 -lopengl32 -lwinmm
#LIBS+=-mno-cygwin
endif

include $(TOP)/mk/target_all.mk

$(TARGET) : $(OBJECTS) ../src/libsgl++.a
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ -L../src $(LIBS)
