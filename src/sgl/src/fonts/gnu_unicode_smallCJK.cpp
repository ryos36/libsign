/* Copyright (c) 2008 Sinby Corporation, All Rights Reserved */

#include "sign/sgl++/sgl.h"
#include "sign/sgl++/fonts/gnu_unicode_smallCJK.h"

extern "C" {
extern const int gnuUnicodeSmallCJKFont[1320512];
}

namespace sign {
namespace sgl {

//------------------------------------------------------
GnuUnicodeSmallCJKFontTable::GnuUnicodeSmallCJKFontTable(void) 
	: FontTable("gnu_unicode_smallCJK", 16, 16, gnuUnicodeSmallCJKFont, FONT_TYPE_BITMAP, FONT_ENC_UTF16LE, FONT_1BPP ) 
{
	size = 32; // bytes
}

//------------------------------------------------------
uint32 
GnuUnicodeSmallCJKFontTable::getCodeWidth(uint32 code)
{
	uint32 index;
	uint32 size;

	index = codeToNo(code);
	if ( code == 0 ) {
		size =  16;
	} else if (( ' ' <= code ) && ( code <= '~' )) {
		size =  8;
	} else if ( index == 0 ) {
		size =  0;
	} else {
		size =  16;
	}
	return size * scaleX;
}

//------------------------------------------------------
uint32
GnuUnicodeSmallCJKFontTable::codeToNo(uint32 code) const 
{
	uint32 no, index;
	if (( 0x0000 <= code ) & ( code <= 0x007f )) {
		index = code;
	} else if (( 0x3040 <= code ) & ( code <= 0x309F )) {
		index = code - 0x3040 + 0x80;
	} else if (( 0x30A0 <= code ) & ( code <= 0x30FF )) {
		index = code - 0x30A0 
			+ ( 0x30A0 - 0x3040 )
			+ 0x80;
	} else if (( 0x31F0 <= code ) & ( code <= 0x31FF )) {
		index = code - 0x31F0 
			+ ( 0x3100 - 0x30A0 )
			+ ( 0x30A0 - 0x3040 )
			+ 0x80;
	} else if (( 0x4E00 <= code ) & ( code <= 0x9FCF )) {
		index = code - 0x4E00 
			+ ( 0x3200 - 0x31F0 )
			+ ( 0x3100 - 0x30A0 )
			+ ( 0x30A0 - 0x3040 )
			+ 0x80;
	} else {
		index = 0;
	}
	return index;
}

//------------------------------------------------------
class GnuUnicodeSmallCJKFontTable gnuUnicodeSmallCJKFontTable;

} // namespace sign
} // namespace sgl

