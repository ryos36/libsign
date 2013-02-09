/* Copyright (c) 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_W_BUFFER_DEVICE_H__
#define __SIGN_SGL_W_BUFFER_DEVICE_H__

#include <string.h>
#include "sign/sign_assert.h"
#include "sign/sgl++/DevicePixel.h"

namespace sign {
namespace sgl {

class WBufferDevice : public GraphicDevice 
{
public:
	typedef void (*CopyFunc) (GraphicDevice *gd0, GraphicDevice *gd1); 
public:
	WBufferDevice(const char *deviceName, GraphicDevice *mainDevice, GraphicDevice *subDevice, CopyFunc copyFunc, CopyFunc rcopyFunc = 0);
	virtual ~WBufferDevice(void){};

	virtual void setPixel(int32 x, int32 y, uint32 color);
	virtual uint32 getPixel(int32 x, int32 y);
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha );
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color);
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha);

	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);
	virtual void putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor );
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha);
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf);

	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);

	virtual void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	virtual void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	virtual void putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );
	virtual void putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );

	virtual void putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor );
	virtual void putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf );

	virtual void getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);
	virtual void repaint(void);
	virtual void backlight(bool on);
	
	void rcopy();
private:
	GraphicDevice *mainDevice, *subDevice;
	CopyFunc copyFunc;
	CopyFunc rcopyFunc;
};

//--------------------------------------------------------------------------
inline
WBufferDevice::WBufferDevice(const char *deviceName, GraphicDevice *mainDevice, GraphicDevice *subDevice, CopyFunc copyFunc, CopyFunc rcopyFunc)
	: GraphicDevice(deviceName, ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y, mainDevice->getMaxSizeX(), mainDevice->getMaxSizeY(), 0, 0, mainDevice->getMaxSizeX(), mainDevice->getMaxSizeY() ),
	  mainDevice(mainDevice),
	  subDevice(subDevice),
	  copyFunc(copyFunc),
	  rcopyFunc(rcopyFunc)
{
	//
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::setPixel(int32 x, int32 y, uint32 color)
{
	subDevice->setPixel(x, y, color);
}

//--------------------------------------------------------------------------
inline
uint32
WBufferDevice::getPixel(int32 x, int32 y)
{
	return subDevice->getPixel(x, y);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color)
{
	subDevice->drawLine(x1, y1, x2, y2, color);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha )
{
	subDevice->drawLine(x1, y1, x2, y2, color, alpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color)
{
	subDevice->fillRectangle(x, y, w, h, color);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha)
{
	subDevice->fillRectangle(x, y, w, h, color, alpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	subDevice->putPattern(x, y, w, h, buf);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor )
{
	subDevice->putPattern(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, transparentColor);
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha)
{
	subDevice->putPattern(x, y, w, h, buf, alpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf)
{
	subDevice->putPattern(x, y, w, h, buf, alphabuf);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	subDevice->putBitPattern(x, y, w, h, buf, fgColor, bgColor);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	subDevice->putBitPattern(x, y, w, h, buf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	subDevice->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, bgColor);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	subDevice->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	subDevice->putPattern8bpp(x, y, w, h, putbuf, fgColor, fgAlpha, bgColor, bgAlpha);
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	subDevice->putPattern8bpp(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	subDevice->putPattern8bppWithPalette(x, y, w, h, srcbuf, palette, transparentColor);
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	subDevice->putPattern8bppWithPalette(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, palette, transparentColor);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf )
{
	subDevice->putPattern32bpp(x, y, w, h, buf );
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor )
{
	subDevice->putPattern32bpp(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, buf, transparentColor);
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf )
{
	subDevice->putPattern32bppWithAlpha(x, y, w, h, buf );
}
//--------------------------------------------------------------------------
inline
void
WBufferDevice::putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf )
{
	subDevice->putPattern32bppWithAlpha(dstX, dstY, dstW, dstH, srcOffsetX, srcOffsetY, srcW, srcH, buf);
}

//--------------------------------------------------------------------------
inline
void
WBufferDevice::getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	subDevice->getPattern(x, y, w, h, buf);
}

//--------------------------------------------------------------------------
void 
WBufferDevice::repaint(void)
{
	(copyFunc)(mainDevice, subDevice);
	mainDevice->repaint();
}

//--------------------------------------------------------------------------
void
WBufferDevice::backlight(bool on)
{
	mainDevice->backlight(on);
}

//--------------------------------------------------------------------------
void
WBufferDevice::rcopy()
{
	if ( rcopyFunc ) {
		(rcopyFunc)(subDevice, mainDevice);
	}
}

//--------------------------------------------------------------------------


} // namespace sgl
} // namespace sign

#endif /* __SIGN_SGL_W_BUFFER_DEVICE_H__ */

