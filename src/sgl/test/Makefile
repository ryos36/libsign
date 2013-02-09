#
# this is for arm -gcc
#

TOP=../../..

include $(TOP)/mk/common.mk

INCS += -I../TestSuite
#CFLAGS += --input-charset=cp932

#
# default OPTS is DEBUG mode
#
LIB_DIRS += -L../src
LIBS += -lsgl++

SOURCES_CPP = \
	LinuxFB.cpp \
	linuxfb_test.cpp 

linuxfb_test:$(OBJECTS)
	$(CXX) -static -o linuxfb_test $(OBJECTS) $(LIB_DIRS) $(LIBS)
