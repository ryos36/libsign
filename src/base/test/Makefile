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
#LIB_DIRS += -L../src
#LIBS += -lsgl++

SOURCES_CPP = 

rc_pointer_test:rc_pointer_test.o
	$(CXX) -static -o rc_pointer_test rc_pointer_test.o $(OBJECTS) $(LIB_DIRS) $(LIBS)


