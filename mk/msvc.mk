# ���� makefile �� gmake �p
# ������ cl ���ĂԁA�A�A
#

#TOP = 

OPTS = -MTd -DDEBUG
OPTS = -MD -DDEBUG
OPTS = -MD 
OPTS = -MDd -DDEBUG
OPTS = -MT 
OPTS = -MT -DDEBUG

OPTS = -MDd -DDEBUG

DEFS = -DWIN32

INCS += -I../../include

#-MD �̓}���`�X���b�h�̓��I�����N�B
# FolderBrowseDialog �� NET 1.1 ���g���Ƃ���,
# ���̃o�O�̂��߂� -MD ���g�p���Ȃ���΂Ȃ炢
#
# �f�o�b�O�łł�  -D_DEBUG �Ƃ̕K�v������ -MDd �ɂ���
# �E�B���h�E�Y�� debug �𗘗p�������Ƃ��� -D_DEBUG �� -MDd ���K�v
#
#-MT �̓}���`�X���b�h�̐ÓI�����N MS ����
#-MTd -TDd �̏������� d �̓f�o�b�O�p
#
#-GX �� C++ �̗�O���� iostream �Ȃǂ��g���Ƃ��ɕK�v
#
#MS_OPTS= -MDd -GX $(WARNING) $(CLR_FLAGS)
#MS_OPTS= -MD -GX $(WARNING) $(CLR_FLAGS)
#MS_OPTS= -MT -GX $(WARNING) $(CLR_FLAGS)
#MS_OPTS= -MTd -GX $(WARNING) $(CLR_FLAGS)

VENDOR_TOP=$(TOP)/../vendor

WARNING=-W1
WARNING=-W3 -wd4800 

MS_OPTS= -GX $(WARNING) $(CLR_FLAGS)

CXXFLAGS = $(MS_OPTS) $(OPTS) $(DEFS) $(INCS) 
CXX=cl
#
CC=cl
CFLAGS = $(OPTS) $(DEFS) $(INCS)
#
GXX=g++
GCXXFLAGS =$(DEFS) $(INCS) -I$(TOP)/gcc_dummy_include -Wall

.SUFFIXES: .cpp .obj .d .c 
.c.obj:
	$(CC) $(CFLAGS) -c $<
.cpp.obj:
	$(CXX) $(CXXFLAGS) -c $<
.cpp.d:
	$(GXX) $(GCXXFLAGS) -MD -E -o .deps/$@.e $<

OBJECTS = $(CPP_SOURCES:.cpp=.obj) $(C_SOURCES:.c=.obj)
CLR_OBJECTS = $(CLR_SOURCES:.cpp=.obj)
DEP_DESC = $(SOURCES:.cpp=.d)

all:$(TARGET)

clean:
	-@rm *.obj $(TARGET)

.PHONY=clean all roundup
