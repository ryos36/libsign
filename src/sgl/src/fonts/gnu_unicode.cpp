/* Copyright (c) 2007 Sinby Corporation, All Rights Reserved */

#include "sign/sgl++/sgl.h"
#include "sign/sgl++/fonts/gnu_unicode.h"

extern "C" {
extern const int gnuUnicodeFont[1320512];
}

namespace sign {
namespace sgl {

//------------------------------------------------------
GnuUnicodeFontTable::GnuUnicodeFontTable(void) 
: FontTable("gnu_unicode", 16, 16, gnuUnicodeFont, FONT_TYPE_BITMAP, FONT_ENC_UTF16LE, FONT_1BPP ) 
{
	size = 16; // bytes
}

//------------------------------------------------------
GnuUnicodeFontTable::~GnuUnicodeFontTable(void)
{
	// nothing to do
}

//------------------------------------------------------
uint32 
GnuUnicodeFontTable::getCodeWidth(uint32 code)
{
	const uint32 *dwp = (const uint32 *)patternTop;
	uint32 s = dwp[code + 1] - dwp[code];
	return s/2 * scaleX;
}

//------------------------------------------------------
uint32
GnuUnicodeFontTable::codeToNo(uint32 code) const 
{
	const uint32 *dwp = (const uint32 *)patternTop;
	uint32 offset = dwp[code];

	return offset/size;
}

//------------------------------------------------------
class GnuUnicodeFontTable GnuUnicodeFontTable;

} // namespace sign
} // namespace sgl

