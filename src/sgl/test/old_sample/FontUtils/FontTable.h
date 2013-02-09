/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_FONT_TABLE_H__
#define __SIGN_SGL_FONT_TABLE_H__

#ifndef _GCC_WCHAR_T
#ifndef __cplusplus
#include <wchar.h>
#endif
#endif
#include "sign/sign_types.h"

namespace sign {
namespace sgl {


enum FontType
{
	FONT_TYPE_BITMAP, //bitmap
	FONT_TYPE_TT,     //true type
	FONT_TYPE_TT_P,   //true type proportional
};

enum FontPixelFormat
{
	FONT_1BPP,
	FONT_8BPP,
};

enum FontEncode
{
	FONT_ENC_ASCII,
	FONT_ENC_SHIFT_JIS,
	FONT_ENC_EUC_JP,
	FONT_ENC_UTF8,
	FONT_ENC_UTF16LE,//little endian
	FONT_ENC_UTF16BE,//big endian
};

class GraphicInfo;

//---------------------------------------------------------------
class FontTable {
public:
	FontTable(const char *fontName, uint32 width, uint32 height, const void *patternTop, 
		FontType fontType, FontEncode encode, FontPixelFormat pixelFormat );
	virtual ~FontTable(void);

	enum {
		FONT_WIDTH_BIT8 = 8,
		FONT_WIDTH_BIT16 = 16,
	};

	const char *getFontName(void) const
	{
		return fontName;
	}

	uint32 getWidth(void) const
	{
		return width;
	}

	uint32 getHeight(void) const
	{
		return height;
	}
	
	virtual void setFontSize( uint32 w, uint32 h )
	{
		scaleX = w/origin_width;
		scaleY = h/origin_height;
		if ( scaleX < 1 ) scaleX = 1;
		if ( scaleY < 1 ) scaleY = 1;
		width  = scaleX * origin_width;
		height = scaleY * origin_height;
	}

	virtual void putPattern( GraphicInfo *graphic, int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, uint32 code );
	//virtual void putPattern( GraphicInfo *graphic, uint32 x, uint32 y, uint32 w, uint32 h, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, uint32 code );
	
	virtual uint32 getCodeWidth( uint32 code );
	
	virtual bool isValidCharCode( uint32 code ){ return false; }

	const FontType fontType;
	const FontEncode encode;
	const FontPixelFormat pixelFormat;

	const uint32 origin_width;
	const uint32 origin_height;

public:
	virtual const uint8 *getPattern(uint32 code);
	virtual uint32 codeToNo(uint32 code) const;

protected:
	const char *fontName;
	uint32 width;
	uint32 height;
	uint32 scaleX;
	uint32 scaleY;
	const void *patternTop;
	uint32 size;
};



//---------------------------------------------------------------
class FontManager {
public:
	enum {
		DEFAULT_FONT = 0,
	};
public:
	virtual ~FontManager() {}

	virtual FontTable *setFontTable(FontTable *fontTable) = 0;
	virtual FontTable *lookupFontTable(const char *fontName = (const char *)DEFAULT_FONT) = 0;
};


} //namespace sign {
} //namespace sgl {

#endif //__SIGN_SGL_FONT_TABLE_H__
