#
# Copyright (C) 2012 Sinby Corp.
#

TOP=../../..

include $(TOP)/mk/common.mk
# CFLAGS += -std=c++0x

#
# default OPTS is DEBUG mode
#

LOCAL_INCS=-I../include

TARGET=xcheme

SOURCES_CPP = \
	WaterPolicy.cpp \
	PairCellAllocator.cpp \
	ExSymbol.cpp \
	ExString.cpp \
	ExBigNum.cpp \
	ExRoot.cpp \
	Environment.cpp \
	StaticSymbol.cpp \
	Number.cpp \
	Token.cpp \
	SimpleTokenReader.cpp \
	SReader.cpp \
	Printer.cpp \
	Scheme.cpp

include $(TOP)/mk/target_all.mk

xcheme: libscheme.a

libscheme.a: $(OBJECTS)
	$(AR) rv $@ $(OBJECTS)						
	
install:libscheme.a
	cp libscheme.a $(INSTALL_DIR)/lib
	cp -rp ../include $(INSTALL_DIR)

-include .deps/PairCellAllocator.d
-include .deps/Environment.d
-include .deps/ExSymbol.d

