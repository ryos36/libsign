# この makefile は gmake 用
# だけど cl を呼ぶ、、、
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

#-MD はマルチスレッドの動的リンク。
# FolderBrowseDialog の NET 1.1 を使うときは,
# そのバグのために -MD を使用しなければならい
#
# デバッグ版では  -D_DEBUG との必要性から -MDd にする
# ウィンドウズの debug を利用したいときは -D_DEBUG と -MDd が必要
#
#-MT はマルチスレッドの静的リンク MS 推奨
#-MTd -TDd の小文字の d はデバッグ用
#
#-GX は C++ の例外処理 iostream などを使うときに必要
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
