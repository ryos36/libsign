/* Copyright (c) 2008 Sinby Corporation, All Rights Reserved */

#define N 5
unsigned int codes[N * 2] = {
	0x0000, 0x007f,
	0x3040, 0x309F, //  Hiragana
	0x30A0, 0x30FF, //  Katakana
	0x31F0, 0x31FF, //  Katakana Phonetic Extensions
	0x4E00, 0x9FCF, //  CJK Unified Ideographs
};

#include <iostream>
#include "./FontTable.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "sign/sgl++/fonts/gnu_unicode.h"

using namespace sign::sgl;

void
print16(uint16 *sp)
{
	int i;
	std::cout << std::hex;
	std::cout << "\t";
	for( i = 0 ; i < 8; ++i, ++sp ) {
		std::cout << "0x" << ((*sp) & 0xFF) << ", ";
		std::cout << "0x" << ((*sp) >> 8 ) << ", ";
	}
	std::cout << std::endl;
	std::cout << "\t";
	for( i = 0 ; i < 8; ++i, ++sp ) {
		std::cout << "0x" << ((*sp) & 0xFF) << ", ";
		std::cout << "0x" << ((*sp) >> 8 ) << ", ";
	}
	std::cout << std::endl;
}

//----------------------------------------------------------------------------
int
main(int argc, char **argv)
{
	uint16 *ui16p;

	FontTable *fontTable;
	fontTable = &GnuUnicodeFontTable;

	/*
	std::cout << fontTable->getWidth() << ":" << fontTable->getHeight() << std::endl;
	std::cout << (void *)fontTable->getPattern(code) << std::endl;
	*/

	for( int i = 0 ; i < N; ++i ) {
		for( uint32 j = codes[i*2]; j <= codes[i*2+1]; ++j ) {
			ui16p = (uint16 *)fontTable->getPattern(j);
			print16(ui16p);
		}
	}
}
