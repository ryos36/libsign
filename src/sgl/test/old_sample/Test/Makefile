#
# this is for arm -gcc
#

TOP=../..

include $(TOP)/mk/common.mk

CFLAGS += --input-charset=cp932

#
# default OPTS is DEBUG mode
#

TARGET=test_on_openGL

SOURCES_CPP = \
	test.cpp \
	$(NULL)

SOURCES_C = assert.c

ifeq ($(OSTYPE), darwin)
LDFLAGS=-framework OpenGL -framework GLUT
LIBS=-lsgl++ 
else
ifeq ($(OSTYPE), cygwin)
CFLAGS+=-mno-cygwin
LIBS=-lsgl++ -lglut32 -lglu32 -lopengl32 
#error not supported
endif
endif


include $(TOP)/mk/target_all.mk

test_on_openGL: $(OBJECTS) ../src/libsgl++.a
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ -L../src $(LIBS)

