#
# this is for arm -gcc
#

TOP=../../..

include $(TOP)/mk/common.mk
PREFIX=arm-none-linux-gnueabi-

INCS += -I../TestSuite
#CFLAGS += --input-charset=cp932

#
# default OPTS is DEBUG mode
#

TARGET = linuxfb_test cvc_ml_clut bitblt0
SOURCES_C = linuxfb_test.c 
SOURCES_CPP = bitblt0.cpp cvc_ml_clut.cpp map_regs.cpp

include $(TOP)/mk/target_all.mk

linuxfb_test:linuxfb_test.o
	$(CC) -o linuxfb_test linuxfb_test.o

cvc_ml_clut:cvc_ml_clut.o map_regs.o
	$(CC) -o cvc_ml_clut cvc_ml_clut.o map_regs.o

bitblt0:bitblt0.o map_regs.o
	$(CXX) -o bitblt0 bitblt0.o map_regs.o -static

.gitignore:Makefile
	echo $(TARGET) | xargs -n 1 > .gitignore
