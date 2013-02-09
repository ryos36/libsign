/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_FRAME_BUFFER_DEVICE_H__
#define __SIGN_SGL_FRAME_BUFFER_DEVICE_H__

#include <string.h>
#include "sign/sign_assert.h"
#include "sign/sgl++/DevicePixel.h"

namespace sign {
namespace sgl {

#define	OVERRUN_CHECK(x) \
		SIGN_ASSERT( (x) <= (buf + (bsize/sizeof(pixel_type))) ) 

#define SAFE_DRAW
#ifdef SAFE_DRAW	

#define CHECK_POS( pos, maxpos ) checkPos( (pos), (maxpos) )
	
inline void	checkPos( int32 &pos, uint32 maxpos )
{
	if ( pos < 0 ) {
		SIGN_ASSERT(0);
		pos = 0;
	} else if ( pos > (int32)maxpos ) {
		SIGN_ASSERT(0);
		pos = maxpos;
	}
}
	

#define CHECK_LENGTH( pos, len, maxsz ) checkLength( (pos), (len), (maxsz) )

inline void checkLength( int32 pos, uint32 &len, uint32 maxsz )
{
	if ( pos+len > maxsz ) {
		SIGN_ASSERT(0);
		len = maxsz - pos;
	}
}
#else
#define CHECK_POS( pos, minpos, maxpos ) SIGN_ASSERT( ((int32)(minpos) <= (pos)) && ((pos) < (int32)(maxpos)) )
#define CHECK_LENGTH( pos, len, maxsz )  SIGN_ASSERT( ((pos)+(len)) <= (maxsz) )
#endif


									   
template< class DevicePixelT >
class FrameBufferDevice : public GraphicDevice 
{
public:
	typedef DevicePixelT device_pixel_type;
	typedef typename DevicePixelT::pixel_type pixel_type;

	FrameBufferDevice(const char *deviceName, uint32 zero_postion, uint32 angle, uint32 direction_spec, int32 maxSizeX, int32 maxSizeY, int32 viewBaseX, int32 viewBaseY, int32 viewSizeX, int32 viewSizeY, pixel_type *frame, bool hasFrame);
	virtual ~FrameBufferDevice(void);

	const pixel_type* getBuffer() const { return buf; }

	virtual void setPixel(int32 x, int32 y, uint32 color);
	virtual uint32 getPixel(int32 x, int32 y);
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha );
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color);
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha);

	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);
	virtual void putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor);
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha);
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf);

	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);

	virtual void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	virtual void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, uint32 bgColor );
	void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, uint32 bgColor );
	void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha );
	void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha );
	virtual void putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );
	virtual void putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );

	virtual void putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor );
	virtual void putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf );

	virtual void getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);
	
	uint32 bsize;
	const uint32 lbsize;

protected:
	pixel_type *buf;
	pixel_type *linebuf;
	uint32 lastw;

private:
	bool hasFrame;
};


//-----------------------------------------------------
template< class DevicePixelT>
FrameBufferDevice<DevicePixelT>::FrameBufferDevice(const char *deviceName, uint32 zero_postion, uint32 angle, uint32 direction_spec, int32 maxSizeX, int32 maxSizeY, int32 viewBaseX, int32 viewBaseY, int32 viewSizeX, int32 viewSizeY, pixel_type *frame = NULL, bool hasFrame = false )
	: GraphicDevice(deviceName, zero_postion, angle, direction_spec, maxSizeX, maxSizeY, viewBaseX, viewBaseY, viewSizeX, viewSizeY) 
	, bsize( maxSizeX * maxSizeY * sizeof(pixel_type) )
	, lbsize( maxSizeX * sizeof(pixel_type) )
	, buf(NULL)
	, linebuf(NULL)
	, lastw(0)
	, hasFrame( hasFrame )
{
	if ( hasFrame ) {
		buf = new pixel_type[ bsize/sizeof(pixel_type) ];
		memset( buf, 0, bsize );
	} else {
		buf = frame;
	}

	linebuf = new pixel_type [lbsize];
	memset(linebuf, 0, lbsize);
	lastw = maxSizeX;
}


//-----------------------------------------------------
template< class DevicePixelT>
FrameBufferDevice<DevicePixelT>::~FrameBufferDevice()
{
	delete linebuf;
	if ( hasFrame ) {
		delete buf;
	}
}


//-----------------------------------------------------
//!1ピクセルを指定位置に描画する
template< class DevicePixelT>
void
FrameBufferDevice<DevicePixelT>::setPixel(int32 x, int32 y, uint32 color)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	
	buf[y * maxSizeX + x] = color;
}


//-----------------------------------------------------
//!1ピクセルを指定位置から取得する
template< class DevicePixelT>
uint32 
FrameBufferDevice<DevicePixelT>::getPixel(int32 x, int32 y)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );

	return buf[y * maxSizeX + x];
}


//-----------------------------------------------------
//線を描画する
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::drawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color )
{
	//領域をはみ出す場合、位置が変更されるため線の角度が変わることがある
	CHECK_POS( x0, maxSizeX-1 );
	CHECK_POS( y0, maxSizeY-1 );
	CHECK_POS( x1, maxSizeX-1 );
	CHECK_POS( y1, maxSizeY-1 );
	

	//ブレセンハムのアルゴリズム
	int dx = ( x1 > x0 ) ? (x1 - x0) : (x0 - x1);
	int dy = ( y1 > y0 ) ? (y1 - y0) : (y0 - y1);
	int x = x0;
	int y = y0;
	int ax = ( x1 >= x0 ) ? 1 : -1;
	int ay = ( y1 >= y0 ) ? 1 : -1;
	
	if ( dx >= dy ) {
		int e = -dx;
		int dy2 = 2*dy;
		int dx2 = 2*dx;
		for ( int i = 0; i <= dx; ++i ) {
			buf[ y * maxSizeX + x ] = color;
			x += ax;
			e += dy2;
			if ( e >= 0 ) {
				y += ay;
				e -= dx2;
			}
		}
	} else {
		int e = -dy;
		int dy2 = 2*dy;
		int dx2 = 2*dx;
		for( int i = 0; i <= dy; ++i ) {
			buf[ y * maxSizeX + x ] = color;
			y += ay;
			e += dx2;
			if ( e >= 0 ) {
				x += ax;
				e -= dy2;
			}
		}
	}

}

//-----------------------------------------------------
//線を描画する
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::drawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color, double alpha )
{
	CHECK_POS( x0, maxSizeX-1 );
	CHECK_POS( y0, maxSizeY-1 );
	CHECK_POS( x1, maxSizeX-1 );
	CHECK_POS( y1, maxSizeY-1 );


	//ブレセンハムのアルゴリズム
	int dx = ( x1 > x0 ) ? (x1 - x0) : (x0 - x1);
	int dy = ( y1 > y0 ) ? (y1 - y0) : (y0 - y1);
	int x = x0;
	int y = y0;
	int ax = ( x1 >= x0 ) ? 1 : -1;
	int ay = ( y1 >= y0 ) ? 1 : -1;

	uint8 alp = (uint8)(alpha*0xFF);
	uint32 r0, g0, b0;
	DevicePixelT::unpack( color, r0, g0, b0 );

	if ( dx >= dy ) {
		int e = -dx;
		int dy2 = 2*dy;
		int dx2 = 2*dx;
		for ( int i = 0; i <= dx; ++i ) {
			//現在のバッファのピクセルとブレンドする
			uint32 r1, g1, b1;
			DevicePixelT::unpack( buf[ y * maxSizeX + x ], r1, g1, b1 );
			buf[ y * maxSizeX + x ] = alphaBlend<DevicePixelT>( r0, g0, b0, alp, r1, g1, b1, 0xFF-alp );
			x += ax;
			e += dy2;
			if ( e >= 0 ) {
				y += ay;
				e -= dx2;
			}
		}
	} else {
		int e = -dy;
		int dy2 = 2*dy;
		int dx2 = 2*dx;
		for( int i = 0; i <= dy; ++i ) {
			//現在のバッファのピクセルとブレンドする
			uint32 r1, g1, b1;
			DevicePixelT::unpack( buf[ y * maxSizeX + x ], r1, g1, b1 );
			buf[ y * maxSizeX + x ] = alphaBlend<DevicePixelT>( r0, g0, b0, alp, r1, g1, b1, 0xFF-alp );
			y += ay;
			e += dx2;
			if ( e >= 0 ) {
				x += ax;
				e -= dy2;
			}
		}
	}

}

//-----------------------------------------------------
//矩形を描画する
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	

	pixel_type *sp;
	uint32 csize;
	
	if ((linebuf[0] != color) || ( lastw < w )) {
		for ( uint32 i = 0 ; i < w; ++i ) {
			linebuf[i] = color;
		}
		lastw = w;
	}
	csize = sizeof(pixel_type) * w;

	sp = &buf[y * maxSizeX + x];
	for ( uint32 i = 0 ; i < h ; ++i, sp += maxSizeX ) {
		memcpy(sp , linebuf, csize);
	}
}


//-----------------------------------------------------
//矩形を描画する
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );


	pixel_type *dp;
	uint8 alp = (uint8)(alpha*0xFF);
	uint32 r0, g0, b0;
	DevicePixelT::unpack( color, r0, g0, b0 );

	for( uint32 yi = y; yi < (y + h); ++yi ) {
		dp = &buf[yi * maxSizeX + x];
		for( uint32 xi = 0; xi < w; ++xi ) {
			uint32 r1, g1, b1;
			DevicePixelT::unpack( *dp, r1, g1, b1 );
			*dp++ = alphaBlend<DevicePixelT>( r0, g0, b0, alp, r1, g1, b1, 0xFF-alp );
			OVERRUN_CHECK(dp);
		}
	}
}


//-----------------------------------------------------
//! DevicePixelTバッファをDevicePixelTバッファに転送する
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *srcbuf)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( srcbuf );


	uint32 csize;
	pixel_type *srcp;
	pixel_type *dstp;

	srcp = static_cast<pixel_type*>(srcbuf);

	csize = sizeof(pixel_type) * w;
	dstp = &buf[y * maxSizeX + x];
	for ( uint32 i = 0 ; i < h ; ++i, srcp += w, dstp += maxSizeX ) {
		memcpy(dstp , srcp, csize);
		OVERRUN_CHECK(dstp);
	}
}


//-----------------------------------------------------
//! DevicePixelTバッファをDevicePixelTバッファに転送する
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	
	SIGN_ASSERT( srcbuf );

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	pixel_type *src = static_cast<pixel_type*>(srcbuf);
	for ( uint32 yi = 0; yi < height; ++yi ) {
		pixel_type *dp = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		pixel_type *sp = &src[ ( srcY + yi ) * srcW + srcX ];
		for ( uint32 xi = 0; xi < width; ++xi ) {
			if ( *sp != transparentColor ) {
				*dp = *sp;
			}
			sp++;
			dp++;
			OVERRUN_CHECK(dp);
		}
	}
}


//-----------------------------------------------------
//! DevicePixelTバッファをDevicePixelTバッファに転送する
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *putbuf, double alpha)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( putbuf );


	pixel_type *dp;
	pixel_type *sp;
	uint8 alp = (uint8)(alpha*0xFF);
	sp = static_cast<pixel_type*>(putbuf);
	
	for( uint32 yi = y; yi < (y + h); ++yi ) {
		dp = &buf[yi * maxSizeX + x];
		for( uint32 xi = 0; xi < w; ++xi, ++sp) {
			uint32 r0, g0, b0, r1, g1, b1;			
			DevicePixelT::unpack( *sp, r0, g0, b0 );
			DevicePixelT::unpack( *dp, r1, g1, b1 );
			*dp++ = alphaBlend<DevicePixelT>( r0, g0, b0, alp, r1, g1, b1, 0xFF-alp );
			OVERRUN_CHECK(dp);
		}
	}
}

//-----------------------------------------------------
//! DevicePixelTバッファをDevicePixelTバッファに転送する
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *putbuf, double *alphabuf)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( putbuf );


	pixel_type *dp;
	pixel_type *sp;
	double *ap;
	sp = static_cast<pixel_type*>(putbuf);
	ap = alphabuf;

	for( uint32 yi = y; yi < (y + h); ++yi ) {
		dp = &buf[yi * maxSizeX + x];
		for( uint32 xi = 0; xi < w; ++xi, ++sp, ++ap) {
			uint32 r0, g0, b0, r1, g1, b1;			
			DevicePixelT::unpack( *sp, r0, g0, b0 );
			DevicePixelT::unpack( *dp, r1, g1, b1 );
			*dp++ = alphaBlend<DevicePixelT>( r0, g0, b0, (uint8)((*ap)*0xFF), r1, g1, b1, (uint8)(0xFF-(*ap)*0xFF) );
			OVERRUN_CHECK(dp);
		}
	}
}





//-----------------------------------------------------
//! 1bppのバッファをDevicePixelTバッファに転送する
//!アルファブレンドなし
//!スケーリングなし
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *bitbuf, uint32 fgColor, uint32 bgColor)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( bitbuf );


	pixel_type *dstp;
	const uint8 *srcp;
	uint32 w8, ws, bytewsize;
	bytewsize = (w + 7)/ 8;
	uint32 color;
	
	for ( uint32 yi = 0 ; yi < h ; ++yi ) {
		dstp = &buf[(y + yi) * maxSizeX + x];
		srcp = &bitbuf[yi * bytewsize];
		for ( uint32 xi = 0; xi < w; xi += 8) {
			w8 =  ( w - xi );
			if ( w8 > 8 ) {
				w8 = 8;
			}
			ws = 0x80;
			for( uint32 wi = 0 ; wi < w8; ++wi ) {
				if (*srcp & ws) {
					color = fgColor;
				} else {
					color = bgColor;
				}
				*dstp = color;
				ws >>= 1;
				++dstp;
				OVERRUN_CHECK(dstp);
			}
			++srcp;
		}
	}
}

//-----------------------------------------------------
//! 1bppのバッファをDevicePixelTバッファに転送する
//!アルファブレンドあり
//!スケーリングなし
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *bitbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( bitbuf );


	if ( (fgAlpha == 1) && (bgAlpha == 1) ){
		putBitPattern( x, y, w, h, bitbuf, fgColor, bgColor );
		return;
	}

	pixel_type *dstp;
	const uint8 *srcp;
	uint32 w8, ws, bytewsize;
	bytewsize = (w + 7)/ 8;
	uint32 color;
	uint8 alpha;

	for( uint32 yi = 0 ; yi < h ; ++yi ) {
		dstp = &buf[(y + yi) * maxSizeX + x];
		srcp = &bitbuf[yi * bytewsize];
		for( uint32 xi = 0; xi < w; xi += 8) {
			w8 =  ( w - xi );
			if ( w8 > 8 ) {
				w8 = 8;
			}
			ws = 0x80;
			for( uint32 wi = 0 ; wi < w8; ++wi ) {
				if (*srcp & ws) {
					color = fgColor;
					alpha = (uint8)(fgAlpha*0xFF);
				} else {
					color = bgColor;
					alpha = (uint8)(bgAlpha*0xFF);
				}
				uint32 r0, g0, b0, r1, g1, b1;
				DevicePixelT::unpack( color, r0, g0, b0 );
				DevicePixelT::unpack( *dstp, r1, g1, b1 );
				*dstp = alphaBlend<DevicePixelT>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
				ws >>= 1;
				++dstp;
				OVERRUN_CHECK(dstp);
			}
			++srcp;
		}
	}
}

//-----------------------------------------------------
//! 1bppのバッファをDevicePixelTバッファに転送する
//!アルファブレンドなし
//!スケーリングあり
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *bitbuf, uint32 fgColor, uint32 bgColor)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	SIGN_ASSERT( bitbuf );


	pixel_type *dstp;
	const uint8 *srcp;
	uint32 ww, ws, bytewsize;
	bytewsize = (w + 7)/ 8;
		
	uint32 width  = w * scaleX;
	uint32 height = h * scaleY;
	
	if ( (x + width) > maxSizeX ) {
		width = maxSizeX - x;
	}
	if ( (y + height) > maxSizeY ) {
		height = maxSizeY - y;
	}

	for ( uint32 yi = 0 ; yi < height; ++yi ) {
		dstp = &buf[(y + yi) * maxSizeX + x];
		srcp = &bitbuf[(yi/scaleY) * bytewsize];

		for ( uint32 xi = 0; xi < width; xi += 8*scaleX) {
			ww =  ( width - xi );
			if ( ww > 8*scaleX ) {
				ww = 8*scaleX;
			}
			ws = 0x80;
			for( uint32 wi = 0, s= 0; wi < ww; ++wi ) {
				++s;
				if (*srcp & (ws>>(s/scaleX)) ) {
					*dstp = fgColor;
				} else {
					*dstp = bgColor;
				}
				++dstp;
				OVERRUN_CHECK(dstp);
			}
			++srcp;
		}
	}
	
}


//-----------------------------------------------------
//! 1bppのバッファをDevicePixelTバッファに転送する
//!アルファブレンドあり
//!スケーリングあり
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *bitbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	SIGN_ASSERT( bitbuf );


	if ( (fgAlpha == 1) && (bgAlpha == 1) ){
		putBitPattern( x, y, w, h, scaleX, scaleY, bitbuf, fgColor, bgColor );
		return;
	}

	pixel_type *dstp;
	const uint8 *srcp;
	uint32 ww, ws, bytewsize;
	bytewsize = (w + 7)/ 8;
	uint32 color;
	uint8 alpha;

	uint32 width  = w * scaleX;
	uint32 height = h * scaleY;

	if ( (x + width) > maxSizeX ) {
		width = maxSizeX - x;
	}
	if ( (y + height) > maxSizeY ) {
		height = maxSizeY - y;
	}

	for( uint32 yi = 0 ; yi < height ; ++yi ) {
		dstp = &buf[(y + yi) * maxSizeX + x];
		srcp = &bitbuf[(yi/scaleY) * bytewsize];
		for( uint32 xi = 0; xi < width; xi += 8*scaleX) {
			ww =  ( width - xi );
			if ( ww > 8*scaleX ) {
				ww = 8*scaleX;
			}
			ws = 0x80;
			for( uint32 wi = 0, s = 0; wi < ww; ++wi ) {
				
				++s;
				if (*srcp & (ws>>(s/scaleX)) ) {
					color = fgColor;
					alpha = (uint8)(fgAlpha*0xFF);
				} else {
					color = bgColor;
					alpha = (uint8)(bgAlpha*0xFF);
				}
				uint32 r0, g0, b0, r1, g1, b1;
				DevicePixelT::unpack( color, r0, g0, b0 );
				DevicePixelT::unpack( *dstp, r1, g1, b1 );
				*dstp = alphaBlend<DevicePixelT>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
				++dstp;
				OVERRUN_CHECK(dstp);
			}
			++srcp;
		}
	}
}


//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定なし
//アルファブレンドあり
//背景色指定あり
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( putbuf );

	if ( fgAlpha ==1.0 && bgAlpha ==1.0 ) {
		putPattern8bpp( x, y, w, h, putbuf, fgColor, bgColor );
		return;
	}
	else if ( bgAlpha == 0 ) {
		putPattern8bpp( x, y, w, h, putbuf, fgColor, fgAlpha );
		return;
	}

	//8bppグレースケールのパターンをpixel_typeに変換
	uint8 fga = (uint8)(fgAlpha*0xFF);
	uint8 bga = (uint8)(bgAlpha*0xFF);
	uint32 rf, gf, bf, rb, gb, bb;
	DevicePixelT::unpack( fgColor, rf, gf, bf );
	DevicePixelT::unpack( bgColor, rb, gb, bb );
	
	for ( uint32 yi = 0; yi < h; ++yi ) {
		pixel_type *dp = &buf[ ( yi + y ) * maxSizeX + x ];
		const uint8 *sp = &putbuf[ yi * w ];
		for ( uint32 xi = 0; xi < w; ++xi ) {
			uint8 alpha = *sp;
			uint8 af = (alpha * fga)>>8;		//foreground alpha
			uint8 ab = ((0xFF-alpha) * bga)>>8;	//background alpha

			//get destination color
			uint32 rd, gd, bd;
			DevicePixelT::unpack( *dp, rd, gd, bd );

			//blend bgcolor
			*dp = alphaBlend<DevicePixelT>( rb, gb, bb, ab, rd, gd, bd, 0xFF-ab );
			
			DevicePixelT::unpack( *dp, rd, gd, bd );

			//blend fgcolor
			*dp++ = alphaBlend<DevicePixelT>( rf, gf, bf, af, rd, gd, bd, 0xFF-af );
			OVERRUN_CHECK(dp);
			
		}
	}
}


//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定なし
//アルファブレンドあり
//背景色指定なし
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( putbuf );

	//8bppグレースケールのパターンをpixel_typeに変換
	uint8 fga = (uint8)(fgAlpha*0xFF);
	uint32 rd, gd, bd;
	uint32 rf, gf, bf;
	DevicePixelT::unpack( fgColor, rf, gf, bf );
		
	for ( uint32 yi = 0; yi < h; ++yi ) {

		pixel_type *dp = &buf[ ( yi + y ) * maxSizeX + x ];
		const uint8 *sp = &putbuf[ yi * w ];

		for ( uint32 xi = 0; xi < w; ++xi ) {
			//get destination color
			DevicePixelT::unpack( *dp, rd, gd, bd );

			uint8 af = ((*sp) * fga)>>8;		//foreground alpha
			//blend fgcolor
			*dp++ = alphaBlend<DevicePixelT>( rf, gf, bf, af, rd, gd, bd, 0xFF-af );
			OVERRUN_CHECK(dp);
			
		}
	}


}


//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定なし
//アルファブレンドなし
//背景色指定あり
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, uint32 bgColor )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( putbuf );


	//8bppグレースケールのパターンを32bppColorのパターンに変換
	uint32 r0, g0, b0, r1, g1, b1;
	DevicePixelT::unpack( fgColor, r0, g0, b0 );
	DevicePixelT::unpack( bgColor, r1, g1, b1 );

	for ( uint32 yi = 0; yi < h; ++yi ) {
		pixel_type *dp = &buf[ ( y + yi ) * maxSizeX + x ];
		const uint8 *sp = &putbuf[ yi * w ];
		for ( uint32 xi = 0; xi < w; ++xi, ++sp ) {
			uint8 alpha = *sp;
			*dp++ = alphaBlend<DevicePixelT>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
			OVERRUN_CHECK(dp);
		}
	}
}

//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定あり
//アルファブレンドあり
//背景色指定なし
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );
	
	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	uint32 rf, gf, bf;
	uint32 rd, gd, bd;
	DevicePixelT::unpack( fgColor, rf, gf, bf );
	
	uint8 fga = (uint8)(fgAlpha*0xFF);
	
	//8bppグレースケールのパターンをpixel_typeに変換
	for ( uint32 yi = 0; yi < height; ++yi ) {
		
		pixel_type *dp = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const uint8 *sp  = &srcbuf[ ( srcY + yi ) * srcW + srcX ];

		for ( uint32 xi = 0; xi < width; ++xi, ++sp ) {
			//get destination color
			DevicePixelT::unpack( *dp, rd, gd, bd );

			uint8 af = ((*sp) * fga)>>8;//foreground alpha

			//blend fgcolor
			*dp++ = alphaBlend<DevicePixelT>( rf, gf, bf, af, rd, gd, bd, 0xFF-af );
			OVERRUN_CHECK(dp);
			
		}
	}

}


//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定あり
//アルファブレンドあり
//背景色指定あり
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );


	if ( fgAlpha ==1.0 && bgAlpha ==1.0 ) {
		putPattern8bpp( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, fgColor, bgColor );
		return;
	}
	else if ( bgAlpha == 0 ) {
		putPattern8bpp( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, fgColor, fgAlpha );
		return;
	}

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	uint32 rf, gf, bf, rb, gb, bb;
	DevicePixelT::unpack( fgColor, rf, gf, bf );
	DevicePixelT::unpack( bgColor, rb, gb, bb );

	uint8 fga = (uint8)(fgAlpha*0xFF);
	uint8 bga = (uint8)(bgAlpha*0xFF);
	//8bppグレースケールのパターンをpixel_typeに変換
	for ( uint32 yi = 0; yi < height; ++yi ) {
		
		pixel_type *dp = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const uint8 *sp  = &srcbuf[ ( srcY + yi ) * srcW + srcX ];

		for ( uint32 xi = 0; xi < width; ++xi, ++sp ) {
			uint8 alpha = *sp;
			uint8 af    = (alpha * fga)>>8;		//foreground alpha
			uint8 ab    = ((0xFF-alpha) * bga)>>8;	//background alpha

			//get destination color
			uint32 rd, gd, bd;
			DevicePixelT::unpack( *dp, rd, gd, bd );

			//blend bgcolor
			*dp = alphaBlend<DevicePixelT>( rb, gb, bb, ab, rd, gd, bd, 0xFF-ab );
			
			DevicePixelT::unpack( *dp, rd, gd, bd );

			//blend fgcolor
			*dp++ = alphaBlend<DevicePixelT>( rf, gf, bf, af, rd, gd, bd, 0xFF-af );
			OVERRUN_CHECK(dp);
			
		}
	}

}

//-----------------------------------------------------
//8bppのグレースケール値のバッファをDevicePixelTバッファに転送する
//転送位置指定あり
//アルファブレンドなし
//背景色指定あり
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, uint32 bgColor )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	//8bppグレースケールのパターンを32bppColorのパターンに変換
	uint32 r0, g0, b0, r1, g1, b1;
	DevicePixelT::unpack( fgColor, r0, g0, b0 );
	DevicePixelT::unpack( bgColor, r1, g1, b1 );

	for ( uint32 yi = 0; yi < height; ++yi ) {
		pixel_type *dp  = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const uint8 *sp  = &srcbuf[ ( srcY + yi ) * srcW + srcX ];

		for ( uint32 xi = 0; xi < width; ++xi, ++sp ) {
			uint8 alpha = *sp;
			*dp++ = alphaBlend<DevicePixelT>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
			OVERRUN_CHECK(dp);
		}
	}
}




//-----------------------------------------------------
//8bpp 256color
//パレットのピクセルフォーマットは
//DevicePixel_A8B8G8R8を期待している
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor  )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( srcbuf );
	SIGN_ASSERT( palette );

	for ( uint32 yi = 0; yi < h; ++yi ) {
		pixel_type *dp  = &buf[ ( y + yi ) * maxSizeX + x ];
		const uint8 *sp = &srcbuf[ yi * w ];
		for ( uint32 xi = 0; xi < w; ++xi ) {
			uint32 srcColor = palette[ *sp++ ];
			if ( srcColor != transparentColor ) {
				uint32 r0, g0, b0;
				DevicePixel_A8B8G8R8::unpack( srcColor, r0, g0, b0 );
				pixel_type pix = DevicePixelT::packNormalize( r0, g0, b0 );
				*dp = pix;
			}
			++dp;
			OVERRUN_CHECK(dp);
		}
	}
}


//-----------------------------------------------------
//転送元位置指定バージョン
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );
	SIGN_ASSERT( palette );

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	for ( uint32 yi = 0; yi < height; ++yi ) {
		pixel_type *dp  = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const uint8 *sp = &srcbuf[ ( srcY + yi ) * srcW + srcX ];
		for ( uint32 xi = 0; xi < width; ++xi ) {
			uint32 srcColor = palette[ *sp++ ];
			if ( srcColor != transparentColor ) {
				uint32 r0, g0, b0;
				DevicePixel_A8B8G8R8::unpack( srcColor, r0, g0, b0 );
				pixel_type pix = DevicePixelT::packNormalize( r0, g0, b0 );
				*dp = pix;
			}
			++dp;
			OVERRUN_CHECK(dp);
		}
	}

}



//-----------------------------------------------------
//32bitビットマップをDevicePixelTバッファに転送する
//ソースのピクセルフォーマットは
//DevicePixel_A8B8G8R8を期待している
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *srcbuf )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( srcbuf );

	for ( uint32 yi = 0; yi < h; ++yi ) {
		pixel_type *dp    = &buf[ (y+yi)*maxSizeX+x ];
		const uint32 *ip  = &srcbuf[ yi*w ];
		for ( uint32 xi = 0; xi < w; ++xi ) {
			uint32 srcColor = *ip;
			uint32 r0, g0, b0;
			DevicePixel_A8B8G8R8::unpack( srcColor, r0, g0, b0 );
			pixel_type pix = DevicePixelT::packNormalize( r0, g0, b0 );
			*dp = pix;
			++dp;
			++ip;
			OVERRUN_CHECK(dp);
		}
	}
}


//-----------------------------------------------------
////特殊化バージョン (TODO:リンクエラー)
template<>
inline
void
FrameBufferDevice<DevicePixel_A8B8G8R8>::putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *srcbuf )
{
	FrameBufferDevice<DevicePixel_A8B8G8R8>::putPattern( x, y, w, h, (void*)srcbuf );
}


//-----------------------------------------------------
//32bitビットマップをDevicePixelTバッファに転送する
//ソースのピクセルフォーマットは
//DevicePixel_A8B8G8R8を期待している
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *srcbuf, uint32 transparentColor )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	for ( uint32 yi = 0; yi < height; ++yi ) {
		pixel_type *dp = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const uint32 *sp = &srcbuf[ ( srcY + yi ) * srcW + srcX ];
		for ( uint32 xi = 0; xi < width; ++xi ) {
			const uint32 srcColor = *sp;
			if ( srcColor != transparentColor ) {
				uint32 r0, g0, b0;
				DevicePixel_A8B8G8R8::unpack( srcColor, r0, g0, b0 );
				pixel_type pix = DevicePixelT::packNormalize( r0, g0, b0 );
				*dp = pix;
			}
			++dp;
			++sp;
			OVERRUN_CHECK(dp);
		}
	}
}

//-----------------------------------------------------
//特殊化バージョン (TODO:リンクエラー)
template <>
inline
void
FrameBufferDevice<DevicePixel_A8B8G8R8>::putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *srcbuf, uint32 transparentColor )
{
	FrameBufferDevice<DevicePixel_A8B8G8R8>::putPattern( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH,  (void*)srcbuf, transparentColor );
}


//-----------------------------------------------------
//32bitビットマップをDevicePixelTバッファに転送する
//ソースのピクセルフォーマットは
//DevicePixel_A8B8G8R8を期待している
//ソースのアルファチャンネルを使用して転送を行う
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *srcbuf )
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( srcbuf );


	pixel_type *dp;
	const DevicePixel_A8B8G8R8::pixel_type *sp;
	sp = reinterpret_cast< const DevicePixel_A8B8G8R8::pixel_type* >(srcbuf);
	
	for( uint32 yi = y; yi < (y + h); ++yi ) {
		dp = &buf[yi * maxSizeX + x];
		for( uint32 xi = 0; xi < w; ++xi, ++sp) {
			uint32 r0, g0, b0, r1, g1, b1;
			DevicePixel_A8B8G8R8::unpack( *sp, r0, g0, b0 );
			DevicePixelT::unpackNormalize( *dp, r1, g1, b1 );
			uint8 alpha = ((*sp & 0xFF000000)>>24);
			
			uint32 d = alphaBlend<DevicePixel_A8B8G8R8>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
			DevicePixel_A8B8G8R8::unpack( d, r0, g0, b0 );
			
			*dp++ = DevicePixelT::packNormalize( r0, g0, b0 );
			OVERRUN_CHECK(dp);
		}
	}
}

//-----------------------------------------------------
//特殊化バージョン  (TODO:リンクエラー)
//template<>
//void
//FrameBufferDevice<DevicePixel_A8B8G8R8>::putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *srcbuf )
//{
//	SIGN_ASSERT( (0 <= x) && ( x < (int32)maxSizeX) );
//	SIGN_ASSERT( (0 <= y) && ( y < (int32)maxSizeY) );
//	SIGN_ASSERT( (x+w) <= maxSizeX );
//	SIGN_ASSERT( (y+h) <= maxSizeY );
//	SIGN_ASSERT( srcbuf );
//
//	uint32 r0, g0, b0, r1, g1, b1;
//	pixel_type *dp;
//	const DevicePixel_A8B8G8R8::pixel_type *sp;
//	sp = reinterpret_cast< const DevicePixel_A8B8G8R8::pixel_type* >(srcbuf);
//	
//	for( uint32 yi = y; yi < (y + h); ++yi ) {
//		dp = &buf[yi * maxSizeX + x];
//		for( uint32 xi = 0; xi < w; ++xi, ++sp) {
//			double alpha = ((*sp & 0xFF000000)>>24)/255.0;
//			DevicePixel_A8B8G8R8::unpack( *sp, r0, g0, b0 );
//			DevicePixel_A8B8G8R8::unpack( *dp, r1, g1, b1 );
//			*dp++ = alphaBlend<DevicePixel_A8B8G8R8>( r0, g0, b0, alpha, r1, g1, b1, 1-alpha );
//			OVERRUN_CHECK(dp);
//		}
//	}
//}

//-----------------------------------------------------
//32bitビットマップをDevicePixelTバッファに転送する
//ソースのピクセルフォーマットは
//DevicePixel_A8B8G8R8を期待している
//ソースのアルファチャンネルを使用して転送を行う
//転送矩形位置を指定する
template< class DevicePixelT >
void 
FrameBufferDevice<DevicePixelT>::putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *srcbuf )
{
	CHECK_POS( dstX, maxSizeX-1 );
	CHECK_POS( dstY, maxSizeY-1 );
	CHECK_POS( srcX, srcW-1 );
	CHECK_POS( srcY, srcH-1 );
	CHECK_LENGTH( dstX, dstW, maxSizeX );
	CHECK_LENGTH( dstY, dstH, maxSizeY );
//	CHECK_LENGTH( srcX, dstW, srcW );
//	CHECK_LENGTH( srcY, dstH, srcH );
	SIGN_ASSERT( srcbuf );

	uint32 width = dstW;
	uint32 height = dstH;
	if ( (srcX+dstW) > srcW ) {
		width = srcW;
	}
	if ( (srcY+dstH) > srcH ) {
		height = srcH;
	}

	uint32 r0, g0, b0, r1, g1, b1;
	const DevicePixel_A8B8G8R8::pixel_type *src;
	src = reinterpret_cast<const DevicePixel_A8B8G8R8::pixel_type*>(srcbuf);
	
	for( uint32 yi = 0; yi < height; ++yi ) {
		pixel_type *dp = &buf[ ( dstY + yi ) * maxSizeX + dstX ];
		const DevicePixel_A8B8G8R8::pixel_type *sp = &src[ ( srcY + yi ) * srcW + srcX ];

		for( uint32 xi = 0; xi < width; ++xi, ++sp) {
			uint8 alpha = ((*sp & 0xFF000000)>>24);
			DevicePixel_A8B8G8R8::unpack( *sp, r0, g0, b0 );
			DevicePixelT::unpackNormalize( *dp, r1, g1, b1 );
			*dp++ = alphaBlend<DevicePixel_A8B8G8R8>( r0, g0, b0, alpha, r1, g1, b1, 0xFF-alpha );
			OVERRUN_CHECK(dp);
		}
	}

}


//-----------------------------------------------------
template< class DevicePixelT >
void
FrameBufferDevice<DevicePixelT>::getPattern(int32 x, int32 y, uint32 w, uint32 h, void *savebuf)
{
	CHECK_POS( x, maxSizeX-1 );
	CHECK_POS( y, maxSizeY-1 );
	CHECK_LENGTH( x, w, maxSizeX );
	CHECK_LENGTH( y, h, maxSizeY );
	SIGN_ASSERT( savebuf );


	uint32 csize;
	pixel_type *sp;
	pixel_type *dp;

	dp = static_cast<pixel_type*>(savebuf);

	csize = sizeof(pixel_type) * w;
	sp = &buf[ y * maxSizeX + x ];
	for ( uint32 i = 0 ; i < h ; ++i, sp += maxSizeX, dp += w ) {
		memcpy(dp , sp, csize);
	}
}


} // namespace sgl
} // namespace sign

#endif /* __SIGN_SGL_FRAME_BUFFER_DEVICE_H__ */

