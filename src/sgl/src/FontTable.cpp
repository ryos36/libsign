#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FontTable.h"

namespace sign {
namespace sgl {

//--------------------------------------------------------
FontTable::FontTable(const char *fontName, uint32 width, uint32 height, const void *patternTop, FontType fontType, FontEncode encode, FontPixelFormat pixelFormat )
: fontType(fontType)
, encode(encode)
, pixelFormat(pixelFormat)
, origin_width(width)
, origin_height(height)
, fontName(fontName)
, width(width)
, height(height)
, scaleX(1)
, scaleY(1)
, patternTop(patternTop)
{
	size = ((origin_width  + FONT_WIDTH_BIT8 - 1)/ FONT_WIDTH_BIT8) * origin_height;
}

//--------------------------------------------------------
FontTable::~FontTable(void)
{
	// nothing to do 
}

//--------------------------------------------------------
//!コード番号に対応する1文字の横幅を得る
uint32
FontTable::getCodeWidth(uint32 code)
{
	return width;
}

//--------------------------------------------------------
//!コード番号からフォントデータ内でのインデックス位置を得る
uint32
FontTable::codeToNo(uint32 code) const
{
	return code;
}


//--------------------------------------------------------

void 
FontTable::putPattern( GraphicInfo *graphic, int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, uint32 code )
{
	const uint8 *bp = getPattern(code);
	if ( (origin_width == dstW) && (origin_height==dstH) ) {
		graphic->putBitPattern( dstX, dstY, origin_width, origin_height, bp, fgColor, fgAlpha, bgColor, bgAlpha );
	}
	else {
		graphic->putBitPattern( dstX, dstY, getCodeWidth(code)/scaleX, origin_height, scaleX, scaleY, bp, fgColor, fgAlpha, bgColor, bgAlpha );
	}
}

//--------------------------------------------------------
//!コード番号に対応する1文字のデータのアドレスを返す
const uint8 *
FontTable::getPattern(uint32 code)
{
	uint32 no;
	const uint8 *bp;
	no = codeToNo(code);

	bp = (const uint8 *)patternTop;

	return &bp[size * no];
}



} // namespace sign
} // namespace sgl
