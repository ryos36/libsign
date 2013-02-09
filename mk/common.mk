#
# since OPTS is DEBUG mode genellary.
# so if you want to compile with -O2 option,
# you should do like this
# ex.
#    make OPTS="-O2"
#
# OPTS=-O2 -gdwarf-2
#

include $(TOP)/install/local.mk

INSTALL_DIR=$(TOP)/install

.SUFFIXES: .E
VENDOR=$(TOP)/../vendor

OPTS=-O0 -DDEBUG -gdwarf-2 -Wall
CXX_OPTS=-std=c++0x
INCS=$(LOCAL_INCS) -I$(INSTALL_DIR)/include

CFLAGS=$(INCS) $(OPTS) $(DEFS) $(CHARSET)
CXXFLAGS=$(CFLAGS) $(CXX_OPTS)
ASFLAGS=-Wa,-gstabs,-gdwarf2,$(ASINCS)

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
AS=$(CC)
AR=$(PREFIX)ar
LEX=flex

#
OBJECTS = $(SOURCES_CPP:.cpp=.o) $(SOURCES_C:.c=.o) $(SOURCES_S:.S=.o)

#
#rules
#
.cpp.E:
	$(CXX) -o $@ -E $(CXXFLAGS) $<
.cpp.o:
	$(CXX) -o $@ -c $(CXXFLAGS) $<
.c.E:
	$(CC) -o $@ -c $(CFLAGS) $<
.c.o:
	$(CC) -o $@ -c $(CFLAGS) $<
.S.o:
	$(AS) -o $@ -c $(ASFLAGS) $<
.l.cpp:
	$(LEX) -o$*.cpp $(LEX_FLAGS)$*.l	

#
#
#

LIB_DIR=$(INSTALL_DIR)/lib
LIB_DIRS=$(PRE_LIB_DIRS) -L$(LIB_DIR)

#
#
#

ifeq ($(PREFIX), mb-)
DEFS += -D__MICROBLAZE__ -DBYTE_ORDER=4321 -DUSE_WSTRING_OF_PUFI
INCS += -I$(TOP)/include/add_include
OPTS = -mno-xl-soft-mul -mcpu=v7.10.d -O2 -g -DDEBUG 
OPTS = -mxl-barrel-shift -DDEBUG -g 
endif

#
# for macintosh
#

ifeq ($(OSTYPE), darwin)
LDFLAGS=-framework OpenGL -framework GLUT
LIBS=-lsgl++ 
endif
