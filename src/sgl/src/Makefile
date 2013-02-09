#
# this is for arm -gcc
#

TOP=../../..

include $(TOP)/mk/common.mk

#
# default OPTS is DEBUG mode
#

LOCAL_INCS=-I../include

CHARSET = --input-charset=cp932

TARGET=libsgl++.a

FONTS_C = \
	fonts/gnu_unicode_data.c \
	fonts/gnu_unicode_smallCJK_data.c \
	$(NULL)

FONTS_CPP = \
        fonts/gnu_unicode.cpp \
        fonts/gnu_unicode_smallCJK.cpp \
	fonts/Hanami.cpp

UTILS_CPP = \
	sjisUtils.cpp \
	$(NULL)

SOURCES_CPP = \
	primitives.cpp \
	sgl.cpp \
	DefaultGraphicDeviceManager.cpp \
	DefaultFontManager.cpp \
	FontTable.cpp \
	$(FONTS_CPP) \
	$(UTILS_CPP) \
	$(NULL)

SOURCES_C = sjis.c moji.c $(FONTS_C)

include $(TOP)/mk/target_all.mk

libsgl++.a: $(OBJECTS)
	$(AR) rv $@ $(OBJECTS)						
	
install:libsgl++.a
	cp libsgl++.a $(INSTALL_DIR)/lib
	cp -rp ../include $(INSTALL_DIR)
