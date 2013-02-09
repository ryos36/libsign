/* Copyright (c) 2008 Sinby Corporation, All Rights Reserved */

#include <iostream>
#include "./FontTable.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "sign/sgl++/fonts/gnu_unicode.h"
#include "sign/sgl++/fonts/gnu_unicode_smallCJK.h"

using namespace sign::sgl;
/*
	&hanamiFontTable);
	&GnuUnicodeFontTable);
*/

extern const unsigned short gnuUnicodeSmallCJKFont[];
//----------------------------------------------------------------------------
void
print8(uint16 ui16)
{
	int v;
	int s;
	s = 0x8000;
	for( int i = 0 ; i < 16; ++i ) {
		v = ui16 & s;
		if ( v ) {
			std::cout << "*";
		} else {
			std::cout << "_";
		}
		s >>= 1;
		if ( i == 7 ) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

//----------------------------------------------------------------------------
void
print16(uint16 ui16)
{
	int v;
	int s;
	s = 0x8000;
	for( int i = 0 ; i < 16; ++i ) {
		v = ui16 & s;
		if ( v ) {
			std::cout << "*";
		} else {
			std::cout << "_";
		}
		s >>= 1;
	}
	std::cout << std::endl;
}

//----------------------------------------------------------------------------
int
main(int argc, char **argv)
{
	uint32 code;
	uint16 *ui16p;
	int width;

	if ( argc == 1 ) {
		/*
		wchar_t *ryos = L"—Ê";
		code = (uint32)*ryos;
		*/
		code = 0x91cf;
	} else {
		code = strtol(argv[1], 0, 16);
	}
	FontTable *fontTable;
	//fontTable = &GnuUnicodeFontTable;
	fontTable = &gnuUnicodeSmallCJKFontTable;

	std::cout << "code:" << std::hex << code << std::dec << std::endl;
	std::cout << fontTable->getWidth() << ":" << fontTable->getHeight() << std::endl;
	width = fontTable->getCodeWidth(code);
	std::cout << "width:" << width << std::endl;
	int no;
	no = fontTable->codeToNo(code);
	std::cout << "no:" << no << std::endl;
	std::cout << (void *)fontTable->getPattern(code) << std::endl;
	ui16p = (uint16 *)fontTable->getPattern(code);
	//std::cout << (void *)&gnuUnicodeSmallCJKFont[no * 16] << std::endl;
	//ui16p = (uint16 *)&gnuUnicodeSmallCJKFont[no * 16];

	if ( width ) {
		for( int i = 0 ; i < 16; ++i, ++ui16p ) {
			uint16 ui16;
			ui16 = *ui16p >> 8;
			ui16 |= (*ui16p & 0xFF) << 8;
			if ( width == 16 ) {
				print16(ui16);
			} else {
				print8(ui16);
				if ( i == 7 ) {
					break;
				}
			}
		}
	} else {
		std::cout << "no data" << std::endl;
	}
}
