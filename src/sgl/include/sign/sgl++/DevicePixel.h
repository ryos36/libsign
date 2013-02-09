/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_DEVICE_PIXEL_H__
#define __SIGN_SGL_DEVICE_PIXEL_H__

#include "sign/sign_types.h"

#if defined(__GNUC__)
#define PACKED	__attribute__((packed))
#elif defined(_MSC_VER)
#define PACKED
#endif


namespace sign {
namespace sgl {


template< class DevicePixelT >
typename DevicePixelT::pixel_type 
alphaBlend( uint32 r0, uint32 g0, uint32 b0, uint8 a0, uint32 r1, uint32 g1, uint32 b1, uint32 a1 )
{
/*
	(x * a + y * (256-a) )/256 =
	(x * a + 256y - ya )/256 =
	(a(x - y) + 256y)/256 =
	a(x - y)/256 + y =
*/
	
	uint8 r, g, b;
	if ( a0 == 0xFF ) {
		r = r0;
		g = g0;
		b = b0;
	}
	else if ( a0 == 0 ) {
		r = r1;
		g = g1;
		b = b1;
	}
	else {
		r = ( (r0 - r1) * a0 >> 8) + r1;
		g = ( (g0 - g1) * a0 >> 8) + g1;
		b = ( (b0 - b1) * a0 >> 8) + b1;
	}

//	r = (uint8)((r0 * a0 + r1 * a1)/255);
//	g = (uint8)((g0 * a0 + g1 * a1)/255);
//	b = (uint8)((b0 * a0 + b1 * a1)/255);
	return DevicePixelT::pack( r, g, b );
}


//---------------------------------------------------------------
// xrrr rrgg gggb bbbb
class DevicePixel_R5G5B5
{
public:
	typedef uint16 pixel_type;
	
	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 0001 1111 -> 0111 1100 0000 0000
		//g: 0000 0000 0001 1111 -> 0000 0011 1110 0000
		//b: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		return  ((r << 10) | (g << 5) | b);
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 0111 1100 0000 0000 -> 0000 0000 0001 1111
		//g: 0000 0011 1110 0000 -> 0000 0000 0001 1111
		//b: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		r = (c & 0x7C00) >> 10;
		g = (c & 0x03E0) >> 5;
		b = (c & 0x001F) >> 0;
	}
	
	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 1111 1000 -> 0111 1100 0000 0000
		//g: 0000 0000 1111 1000 -> 0000 0011 1110 0000
		//b: 0000 0000 1111 1000 -> 0000 0000 0001 1111
		return ((r & 0xF8) << 7) | ((g & 0xF8) << 2) | ((g & 0xF8) >> 3);
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 0111 1100 0000 0000 -> 0000 0000 1111 1000
		//g: 0000 0011 1110 0000 -> 0000 0000 1111 1000
		//b: 0000 0000 0001 1111 -> 0000 0000 1111 1000
		r = ((c&0x7C00) >> 7);
		g = ((c&0x03E0) >> 2);
		b = ((c&0x001F) << 3);
	}
};


//---------------------------------------------------------------
// xbbb bbgg gggr rrrr
class DevicePixel_B5G5R5
{
public:
	typedef uint16 pixel_type;
	
	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		//g: 0000 0000 0001 1111 -> 0000 0011 1110 0000
		//b: 0000 0000 0001 1111 -> 0111 1100 0000 0000
		return  (r | (g << 5) | (b << 10));
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		//g: 0000 0011 1110 0000 -> 0000 0000 0001 1111
		//b: 0111 1100 0000 0000 -> 0000 0000 0001 1111
		r = (c & 0x001F) >> 0;
		g = (c & 0x03E0) >> 5;
		b = (c & 0x7C00) >> 10;
	}
	
	//各色の入力値は 0-255で指定する
	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 1111 1000 -> 0000 0000 0001 1111
		//g: 0000 0000 1111 1000 -> 0000 0011 1110 0000
		//b: 0000 0000 1111 1000 -> 0111 1100 0000 0000
		return ((r&0xF8)>>3) | ((g&0xF8)<<2) | ((b&0xF8)<<7);
	}

	//各色は 0-255に正規化される
	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 0000 0000 0001 1111 -> 0000 0000 1111 1000
		//g: 0000 0011 1110 0000 -> 0000 0000 1111 1000
		//b: 0111 1100 0000 0000 -> 0000 0000 1111 1000
		r = ((c&0x001F) << 3);
		g = ((c&0x03E0) >> 2);
		b = ((c&0x7C00) >> 7);
	}
};


//---------------------------------------------------------------
// rrrr rggg gggb bbbb
class DevicePixel_R5G6B5
{
public:
	typedef uint16 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 0001 1111 -> 1111 1000 0000 0000
		//g: 0000 0000 0011 1111 -> 0000 0111 1110 0000
		//b: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		return  ((r << 11) | (g << 5) | b);
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 1111 1000 0000 0000 -> 0000 0000 0001 1111
		//g: 0000 0111 1110 0000 -> 0000 0000 0011 1111
		//b: 0000 0000 0001 1111 -> 0000 0000 0001 1111
		r = ( c & 0xF800 ) >> 11;
		g = ( c & 0x07E0 ) >> 5;
		b = ( c & 0x001F ) >> 0;
	}

	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		//r: 0000 0000 1111 1000 -> 1111 1000 0000 0000
		//g: 0000 0000 1111 1100 -> 0000 0111 1110 0000
		//b: 0000 0000 1111 1000 -> 0000 0000 0001 1111
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		//r: 1111 1000 0000 0000 -> 0000 0000 1111 1000
		//g: 0000 0111 1110 0000 -> 0000 0000 1111 1100
		//b: 0000 0000 0001 1111 -> 0000 0000 1111 1000
		r = ((c & 0xF800) >> 8);
		g = ((c & 0x07E0) >> 3);
		b = ((c & 0x001F) << 3);
	}
};




//---------------------------------------------------------------
// aaaa aaaa rrrr rrrr gggg gggg bbbb bbbb
class DevicePixel_A8R8G8B8
{
public:
	typedef uint32 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		return  ((r << 16) | (g << 8) | b);
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		r = ( c & 0x00FF0000 ) >> 16;
		g = ( c & 0x0000FF00 ) >> 8;
		b = ( c & 0x000000FF ) >> 0;
	}


	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return pack( r, g, b );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		unpack( c, r, g, b );
	}

};





//---------------------------------------------------------------
// aaaa aaaa bbbb bbbb gggg gggg rrrr rrrr
class DevicePixel_A8B8G8R8
{
public:
	typedef uint32 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		return  (b << 16) | (g <<8) | r;
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		r = ( c & 0x000000FF ) >> 0;
		g = ( c & 0x0000FF00 ) >> 8;
		b = ( c & 0x00FF0000 ) >> 16;
	}


	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return pack( r, g, b );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		unpack( c, r, g, b );
	}
};



//---------------------------------------------------------------
// rrrr rrrr gggg gggg bbbb bbbb aaaa aaaa
class DevicePixel_R8G8B8A8
{
public:
	typedef uint32 pixel_type;
	
	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		return  ( ( r << 24 ) | ( g << 16 ) | ( b << 8 ) );
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		r = ( c & 0xFF000000 ) >> 24;
		g = ( c & 0x00FF0000 ) >> 16;
		b = ( c & 0x0000FF00 ) >> 8;
	}


	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return pack( r, g, b );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		unpack( c, r, g, b );
	}
};



//---------------------------------------------------------------
// rrrr rrrr gggg gggg bbbb bbbb aaaa aaaa
class DevicePixel_B8G8R8A8
{
public:
	typedef uint32 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		return  ( ( b << 24 ) | ( g << 16 ) | ( r << 8 ) );
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		r = ( c & 0x0000FF00 ) >> 8;
		g = ( c & 0x00FF0000 ) >> 16;
		b = ( c & 0xFF000000 ) >> 24;
	}


	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return pack( r, g, b );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		unpack( c, r, g, b );
	}
};

//----------------------------------------------------------------------------
// xxxx_xxrr rrrr_gggg ggbb_bbbb 
class DevicePixel_R6G6B6
{
public:
	struct fb24 {
		uint8 buf[3];	

		fb24(){}

		fb24(uint8 r, uint8 g, uint8 b) {
			buf[0] = b;
			buf[0] |= ((g&0x3)  << 6);
			buf[1] =  ((g&0x3C) >> 2);
			buf[1] |= ((r&0xF)  << 4);
			buf[2] =  ((r&0x30) >> 4);
		}

		fb24 &operator=(uint32 i) {
			uint8 r, g, b;
			b = (i & 0x00FF0000) >> 16;
			g = (i & 0x0000FF00) >> 8;
			r = (i & 0x000000FF) >> 0;
			
			buf[0] = b;
			buf[0] |= ((g&0x3)  << 6);
			buf[1] =  ((g&0x3C) >> 2);
			buf[1] |= ((r&0xF)  << 4);
			buf[2] =  ((r&0x30) >> 4);
			
			return (*this);
		}

		operator const int32() const
		{
			return (buf[2] <<16) | (buf[1] <<8) | buf[0];
		}
		
	} PACKED;

	typedef struct fb24 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		return fb24(r, g, b);
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		b = (c.buf[0]&0x3F);
		g = ((c.buf[0]&0xC0)>>6) | ((c.buf[1]&0x0F)<<2);
		r = ((c.buf[1]&0xF0)>>4) | ((c.buf[2]&0x03)<<4); 
	}

	inline static void
	unpack( uint32 c, uint32 &r, uint32 &g, uint32 &b )
	{
		b = (c & 0x00FF0000) >> 18;
		g = (c & 0x0000FF00) >> 10;
		r = (c & 0x000000FF) >> 2;
	}
	
	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return fb24( r>>2, g>>2, b>>2 );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		b =   (c.buf[0]&0x3F) << 2;
		g = (((c.buf[0]&0xC0)>>6) | ((c.buf[1]&0x0F)<<2)) << 2;
		r = (((c.buf[1]&0xF0)>>4) | ((c.buf[2]&0x03)<<4)) << 2; 
	}

};

//----------------------------------------------------------------------------
// with color table
template< class ColorTable >
class DevicePixel_8BPP
{
public:
	enum {
		INVALID_COLOR = 0xFFFFFFFF
	};
	typedef uint8 pixel_type;

	inline static pixel_type 
	pack( uint8 r, uint8 g, uint8 b )
	{
		uint32 color;
		color = ColorTable::findColor(r, g, b);
		// transparent : TODO
		if ( color == INVALID_COLOR ) {
			return 0; // invalid color!!
		}
		return  (uint8) color;
	}

	inline static void 
	unpack( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		uint32 color;
		color = ColorTable::findColor(c);
		// argb
		r = (color & 0xFF0000) >> 16;
		g = (color & 0x00FF00) >>  8;
		b = (color & 0x0000FF) >>  0;
	}

	inline static pixel_type
	packNormalize( uint8 r, uint8 g, uint8 b )
	{
		return pack( r, g, b );
	}

	inline static void 
	unpackNormalize( pixel_type c, uint32 &r, uint32 &g, uint32 &b )
	{
		unpack( c, r, g, b );
	}
};

//----------------------------------------------------------------------------

} // namespace sign
} // namespace sgl

#endif //__SIGN_SGL_DEVICE_PIXEL_H__

