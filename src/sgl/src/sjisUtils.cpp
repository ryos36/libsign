/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */

#include <locale.h>
#include <stdlib.h>
#include "sign/sign_types.h"

#define etoj(c1, c2) {c1 &= 0x7f; c2 &= 0x7f;}

#if defined(_MSC_VER)
	#ifdef _M_IX86
		#define HAS_WCTOMB
		#define HAS_MBTOWC
	#endif
#endif

//----------------------------------------------------------------------------
//utility
/*
 * @deprecated 
 */
bool
isShiftJisMultiByteChar( const char c )
{
	const unsigned char	b = (const unsigned char)c;
	if ( ( ( b > 128 ) && ( b < 160 ) ) ||
		( ( b > 223 ) && ( b < 255 ) ) ) {
		return true;
	}
	else {
		return false;
	}

}

//----------------------------------------------------------------------------
//utility
/*
 */
bool
isZenkakuKatakana( uint32 code )
{
	if ((( 0x8340 <= code ) && ( 0x8396 >= code )) ||
	    ( 0x815B == code )) {

		return true;
	} else {
		return false;
	}
}

//----------------------------------------------------------------------------
//utility
/*
 */
bool
isZenkakuHiragana( uint32 code )
{
	if ((( 0x82a0 <= code ) && ( 0x82f1 >= code ))) {
		return true;
	} else {
		return false;
	}
}

//----------------------------------------------------------------------------
uint32
unicode2sjis( uint32 code )
{
#ifdef HAS_WCTOMB
	setlocale( LC_ALL, "Japanese" );
	char buf[4] = {0,0,0,0};
	wctomb( buf, code );

	if ( isShiftJisMultiByteChar( buf[0] ) ) {
		return ( (unsigned char)buf[1] | ( (unsigned char)buf[0] << 8) );
	}
	else {
		return buf[0] & 0xFF;
	}
#else
	return code;
#endif
}

//----------------------------------------------------------------------------
uint32
sjis2unicode( uint32 code )
{
#ifdef HAS_MBTOWC
	wchar_t wc;
	char buf[2];
	setlocale( LC_ALL, "Japanese" );
	if ( isShiftJisMultiByteChar( (code>>8) ) ) {
		buf[0] = (code>>8) & 0xFF;
		buf[1] = code      & 0xFF;
		mbtowc( &wc, buf, 2 );
	}
	else {
		buf[0] = code;
		mbtowc( &wc, buf, 1 );
	}
		
	return wc;
#else
	return code;
#endif
}

//----------------------------------------------------------------------------
uint32 
getShiftJisCode( const char *c_str, uint32 *code )
{
	if ( isShiftJisMultiByteChar(*c_str) ) {
		if ( c_str[1] ) {
			*code = ( (unsigned char)c_str[1] | ( (unsigned char)c_str[0] << 8) );
			return 2;
		} else {
			*code = 0;
			return 1;	
		}
	}
	else {
		*code = (unsigned char)c_str[0];
		return 1;
	}
}


//----------------------------------------------------------------------------

