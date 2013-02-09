#
#
#
TOP=../..

include $(TOP)/mk/common.mk

#PREFIX=arm-linux-

TARGET=OpenGLDevice.o

SOURCES_CPP = \
	OpenGLDevice.cpp

include $(TOP)/mk/target_all.mk

install:
	$(AR) rv ../src/libsgl++.a $(OBJECTS)
