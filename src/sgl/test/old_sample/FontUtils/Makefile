#
# this is for arm -gcc
#

TOP=../..

include $(TOP)/mk/common.mk

#CFLAGS += --input-charset=cp932

#
# default OPTS is DEBUG mode
#

TARGET=decodeFont

SOURCES_CPP = \
	decodeFont.cpp \
	$(NULL)


LIBS=-lsgl++


include $(TOP)/mk/target_all.mk

decodeFont: $(OBJECTS) ../src/libsgl++.a
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ -L../src $(LIBS)

mksmallCJKfont: mksmallCJKfont.o ../src/libsgl++.a
	$(CXX) $(LDFLAGS) mksmallCJKfont.o -o $@ -L../src $(LIBS)

