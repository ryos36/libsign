/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_INC__
#define __SIGN_SGL_INC__

namespace sign {
namespace sgl {


//--------------------------------------------------------

void
GraphicInfo::setPixel(int32 x, int32 y, uint32 color)
{
	lock();
	device->setPixel(x, y, color);
	unlock();
}

uint32 
GraphicInfo::getPixel(int32 x, int32 y)
{
	return device->getPixel(x,y);
}

//--------------------------------------------------------

void 
GraphicInfo::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color)
{
	lock();
	device->drawLine( x1, y1, x2, y2, color );
	unlock();
}

//--------------------------------------------------------

void 
GraphicInfo::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha )
{
	lock();
	device->drawLine( x1, y1, x2, y2, color, alpha );
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color)
{
	lock();
	device->fillRectangle(x, y, w, h, color);
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha)
{
	if ( alpha == 1.0 ) {
		device->fillRectangle(x, y, w, h, color);
	} else if ( alpha == 0.0 ) {
		return;
	}

	lock();
	device->fillRectangle(x, y, w, h, color, alpha);
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	lock();
	device->putPattern(x, y, w, h, buf);
	unlock();
}


//--------------------------------------------------------
void 
GraphicInfo::putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor )
{
	lock();
	device->putPattern(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, transparentColor);
	unlock();
}


//--------------------------------------------------------

void
GraphicInfo::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha)
{
	if ( alpha == 1.0 ) {
		device->putPattern(x, y, w, h, buf);
	} else if ( alpha == 0.0 ) {
		return;
	}
	lock();
	device->putPattern(x, y, w, h, buf, alpha);
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf)
{
	lock();
	device->putPattern(x, y, w, h, buf, alphabuf);
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	lock();
	device->putBitPattern(x, y, w, h, buf, fgColor, bgColor);
	unlock();
}



//--------------------------------------------------------

void
GraphicInfo::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	lock();
	device->putBitPattern(x, y, w, h, buf, fgColor, fgAlpha, bgColor, bgAlpha);
	unlock();
}

//--------------------------------------------------------
void 
GraphicInfo::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	lock();
	device->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, bgColor);
	unlock();
}

//--------------------------------------------------------
void 
GraphicInfo::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	lock();
	device->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, fgAlpha, bgColor, bgAlpha);
	unlock();
}

//--------------------------------------------------------

void 
GraphicInfo::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	lock();
	device->putPattern8bpp(x, y, w, h, putbuf, fgColor, fgAlpha, bgColor, bgAlpha );
	unlock();
}


//--------------------------------------------------------
void 
GraphicInfo::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	lock();
	device->putPattern8bpp( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, putbuf, fgColor, fgAlpha, bgColor, bgAlpha );
	unlock();
}


//--------------------------------------------------------
void 
GraphicInfo::putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	lock();
	device->putPattern8bppWithPalette( x, y, w, h, srcbuf, palette, transparentColor );
	unlock();
}

//--------------------------------------------------------
void 
GraphicInfo::putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	lock();
	device->putPattern8bppWithPalette( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, palette, transparentColor );
	unlock();
}


//----------------------------------------------------------------- 
void
GraphicInfo::putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *srcbuf )
{
	lock();
	device->putPattern32bpp( x, y, w, h, srcbuf );
	unlock();
}


//----------------------------------------------------------------- 
void
GraphicInfo::putPattern32bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor )
{
	lock();
	device->putPattern32bpp( dstX, dstY, dstW, dstH, srcOffsetX, srcOffsetY, srcW, srcH, buf, transparentColor );
	unlock();
}

//----------------------------------------------------------------- 
void 
GraphicInfo::putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf )
{
	lock();
	device->putPattern32bppWithAlpha(x, y, w, h, buf );
	unlock();
}

//----------------------------------------------------------------- 
void 
GraphicInfo::putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf )
{
	lock();
	device->putPattern32bppWithAlpha(dstX, dstY, dstW, dstH, srcOffsetX, srcOffsetY, srcW, srcH, buf );
	unlock();

}





//--------------------------------------------------------
void
GraphicInfo::getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	lock();
	device->getPattern(x, y, w, h, buf);
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::repaint(void)
{
	lock();
	device->repaint();
	unlock();
}

//--------------------------------------------------------

void
GraphicInfo::backlight(bool on)
{
	lock();
	device->backlight(on);
	unlock();
}



} // namespace sgl
} // namespace sign

#endif /* __SIGN_SGL_INC__ */
