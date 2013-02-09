/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */

#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FontTable.h"
#include "sign/sign_assert.h"

extern "C" {
unsigned short sjis2jis(unsigned int sjis);
unsigned int Jisx0208ToUnicode11(unsigned int jis);
}
uint32 getShiftJisCode( const char *c_str, uint32 *code );
uint32 sjis2unicode( uint32 code );

namespace sign {
namespace sgl {

//--------------------------------------------------------
GraphicDevice::GraphicDevice(const char *deviceName, uint32 zero_postion, uint32 angle, uint32 direction_spec, uint32 maxSizeX, uint32 maxSizeY, int32 viewBaseX, int32 viewBaseY, uint32 viewSizeX, uint32 viewSizeY ) 
{
	this->deviceName = (char *)deviceName;
	this->status = 
		((zero_postion & STATUS_ZERO_P_MASK) << STATUS_ZERO_P_SHIFT) |
		((angle & STATUS_ANGLE_MASK) << STATUS_ANGLE_SHIFT) |
		((direction_spec & STATUS_HAS_D_SPEC_MASK) << STATUS_HAS_D_SPEC_SHIFT);

	this->maxSizeX = maxSizeX;
	this->maxSizeY = maxSizeY;

	this->viewBaseX = viewBaseX;
	this->viewBaseY = viewBaseY;

	this->viewSizeX = viewSizeX;
	this->viewSizeY = viewSizeY;
}

//--------------------------------------------------------
bool
GraphicDevice::setDeviceStatus(uint32 status)
{
	return true;
}

//--------------------------------------------------------
EventManager::EventManager(void)
{
	keyListener = 0;
	pointingDeviceListener = 0;
}

//--------------------------------------------------------
void
EventManager::setKeyboardListener(KeyboardListener *listener)
{
	this->keyListener = listener;
}

//--------------------------------------------------------
void
EventManager::setPointingDeviceListener(PointingDeviceListener *listener)
{
	pointingDeviceListener = listener;
}

//--------------------------------------------------------
GraphicDeviceManager * GraphicInfo::manager = 0;
FontManager *GraphicInfo::fontManager = 0;
EventManager *GraphicInfo::eventManager = 0;


//--------------------------------------------------------
GraphicInfo::GraphicInfo( const char *deviceName )
{
	device = manager->lookupDevice(deviceName);
	SIGN_ASSERT( device );
	fontTable = fontManager->lookupFontTable();
	SIGN_ASSERT( fontTable );

#ifdef _MSC_VER
	::InitializeCriticalSection( &cs );
#endif

}


//--------------------------------------------------------
GraphicInfo::~GraphicInfo(void)
{
	// nothing to do
}

//--------------------------------------------------------
void
GraphicInfo::setKeyboardListener(KeyboardListener *listener)
{
	GraphicInfo::eventManager->setKeyboardListener(listener);
}

//--------------------------------------------------------
void
GraphicInfo::doEventLoop(void)
{
	if ( eventManager ) {
		GraphicInfo::eventManager->doEventLoop();
	} else {
		while(true) {
			assert(0);
			// TODO System msleep
			//::msleep(500);
		}
	}
}

//--------------------------------------------------------
void
GraphicInfo::setPointingDeviceListener(PointingDeviceListener *listener)
{
	GraphicInfo::eventManager->setPointingDeviceListener(listener);
}

//--------------------------------------------------------
int32
GraphicInfo::getModifiers(void)
{
	return GraphicInfo::eventManager->getModifiers();
}

//--------------------------------------------------------
void
GraphicInfo::msleep(unsigned int miliseconds)
{
	repaint();
	GraphicInfo::eventManager->msleep(miliseconds);
	repaint();
}

//--------------------------------------------------------
void 
GraphicInfo::setGraphicDeviceManager(GraphicDeviceManager *manager)
{
	GraphicInfo::manager = manager;
}

//--------------------------------------------------------
void 
GraphicInfo::setFontManager(FontManager *fontManager)
{
	GraphicInfo::fontManager = fontManager;
}

//--------------------------------------------------------
void 
GraphicInfo::setEventManager(EventManager *eventManager)
{
	GraphicInfo::eventManager = eventManager;
}

//--------------------------------------------------------
void
GraphicInfo::lock(void)
{
	// nothing to do
#ifdef _MSC_VER
	::EnterCriticalSection( &cs );
#endif
}

//--------------------------------------------------------
void
GraphicInfo::unlock(void)
{
	// nothing to do
#ifdef _MSC_VER
	::LeaveCriticalSection( &cs );
#endif
}


//--------------------------------------------------------
//! ������̕`����s��
/*
	@param[in] x		�`��J�n�ʒu��X���W
	@param[in] y		�`��J�n�ʒu��Y���W
	@param[in] str		�`�悷�镶����
	@param[in] fgColor	�����̐F
	@param[in] bgColor	�����̔w�i�F
*/
void
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, uint32 bgColor)
{
	drawString( x, y, str, fgColor, 1.0, bgColor, 1.0 );
}

//--------------------------------------------------------
//! ������̕`����s��
/*
	@param[in] x		�`��J�n�ʒu��X���W
	@param[in] y		�`��J�n�ʒu��Y���W
	@param[in] str		�`�悷�镶����
	@param[in] fgColor	�����̐F
	@param[in] fgAlpha	�����̃A���t�@�l
	@param[in] bgColor	�����̔w�i�F
	@param[in] bgAlpha	�w�i�F�̃A���t�@�l
*/
void
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{

	uint32 wi, hi;
	int32 xi = x;
	hi = fontTable->getHeight();
	uint32 maxx = device->getMaxSizeX();

	const char *cp = str;
	while( *cp ) {
	
		uint32 code;
		int len = getShiftJisCode( cp, &code );
		cp += len;
		code = sjis2unicode(code);
		
		wi = fontTable->getCodeWidth(code);
		
		fontTable->putPattern( this, xi, y, wi, hi, 0, 0, wi, hi, fgColor, fgAlpha, bgColor, bgAlpha, code );
		if ( (xi+wi) >= maxx ) {
			break;
		}
		xi += wi;
		SIGN_ASSERT ( xi < (int32)maxx );
		
	}
	
}

//--------------------------------------------------------
//! ������̕`����s��
/*
	@param[in] x		�`��J�n�ʒu��X���W
	@param[in] y		�`��J�n�ʒu��Y���W
	@param[in] w		�����̕�  (�g�p����t�H���g�ɂ���Ďw��ł���l�͈قȂ�)
	@param[in] h		�����̍��� (�g�p����t�H���g�ɂ���Ďw��ł���l�͈قȂ�)
	@param[in] str		�`�悷�镶����
	@param[in] fgColor	�����̐F
	@param[in] bgColor	�����̔w�i�F
*/
void
GraphicInfo::drawString( int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, uint32 bgColor )
{
	drawString( x, y, w, h, str, fgColor, 1.0, bgColor, 1.0 );
}


//--------------------------------------------------------
//! ������̕`����s��
void
/*
	@param[in] x		�`��J�n�ʒu��X���W
	@param[in] y		�`��J�n�ʒu��Y���W
	@param[in] w		�����̕�  (�g�p����t�H���g�ɂ���Ďw��ł���l�͈قȂ�)
	@param[in] h		�����̍��� (�g�p����t�H���g�ɂ���Ďw��ł���l�͈قȂ�)
	@param[in] str		�`�悷�镶����
	@param[in] fgColor	�����̐F
	@param[in] fgAlpha	�����̃A���t�@�l
	@param[in] bgColor	�����̔w�i�F
	@param[in] bgAlpha	�w�i�F�̃A���t�@�l
*/
GraphicInfo::drawString( int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	fontTable->setFontSize( w, h );
	drawString( x, y, str, fgColor, fgAlpha, bgColor, bgAlpha );
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, uint32 bgColor, const Rect &clipRect )
{
	drawString( x, y, str, fgColor, 1.0, bgColor, 1.0, clipRect );
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect )
{
	int32 fontH = fontTable->getHeight();
	if ( (y + fontH) < clipRect.top ) {
		return;
	}
	if ( y > clipRect.bottom ) {
		return;
	}

	uint32 maxx = device->getMaxSizeX();
	SIGN_ASSERT( clipRect.right <= (int)maxx );

	int32 xi = x;
	const char *cp = str;
	while( *cp ) {
	
		uint32 code;
		int len = getShiftJisCode( cp, &code );
		cp += len;
		code = sjis2unicode(code);

		int fontW = fontTable->getCodeWidth(code);

		//cliping
		Rect drawRect ( xi, fontW + xi, y, fontH + y );
		Rect rect = drawRect.intersection( clipRect );
		rect.left -= xi;
		rect.right -= xi;
		rect.top -= y;
		rect.bottom -= y;
		SIGN_ASSERT( 0 <= rect.left );
		SIGN_ASSERT( 0 <= rect.top );
		if ( !rect.empty() ) {
			fontTable->putPattern( this, xi + rect.left, y + rect.top, rect.width(), rect.height(), rect.left, rect.top, fontW, fontH, fgColor, fgAlpha, bgColor, bgAlpha, code );
		}
		else {
			if ( xi >= clipRect.right ) {
				break;
			}
		}

		if ( (xi+fontW) >= (int)maxx ) {
			break;
		}
		xi += fontW;
		
	}
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, uint32 bgColor, const Rect &clipRect )
{
	drawString( x, y, w, h, str, fgColor, 1.0, bgColor, 1.0, clipRect );
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect )
{
	fontTable->setFontSize( w, h );
	drawString( x, y, str, fgColor, fgAlpha, bgColor, bgAlpha, clipRect );
}


//wchar_t version/////////////////////////////////////////
//--------------------------------------------------------
void
GraphicInfo::drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, uint32 bgColor)
{
	drawString( x, y, wstr, fgColor, 1.0, bgColor, 1.0 );
}

//--------------------------------------------------------
// code is unicode
void
GraphicInfo::drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	int32 wi, hi;
	int32 xi = x;
	hi = fontTable->getHeight();
	const wchar_t *wcp = wstr;
	uint32 maxx = device->getMaxSizeX();
		
	while( *wcp ) {
		uint32 code = *wcp;

		wi = fontTable->getCodeWidth(code);
				
		fontTable->putPattern( this, xi, y, wi, hi, 0, 0, wi, hi, fgColor, fgAlpha, bgColor, bgAlpha, code );
		if ( (xi+wi) >= (int32)maxx ) {
			break;
		}

		xi += wi;
		SIGN_ASSERT ( xi < (int32)maxx );
		++wcp;
		
	}
}

//--------------------------------------------------------
void
GraphicInfo::drawString( int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, uint32 bgColor )
{
	drawString( x, y, w, h, wstr, fgColor, 1.0, bgColor, 1.0 );
}

//--------------------------------------------------------
void
GraphicInfo::drawString( int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	fontTable->setFontSize( w, h );
	drawString( x, y, wstr, fgColor, fgAlpha, bgColor, bgAlpha );
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, uint32 bgColor, const Rect &clipRect )
{
	drawString( x, y, wstr, fgColor, 1.0, bgColor, 1.0, clipRect );
}

//--------------------------------------------------------
// code is unicode
void 
GraphicInfo::drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect )
{
	int32 fontH = fontTable->getHeight();
	if ( (y + fontH) < clipRect.top ) {
		return;
	}
	if ( y > clipRect.bottom ) {
		return;
	}

	uint32 maxx = device->getMaxSizeX();
	SIGN_ASSERT( clipRect.right <= (int)maxx );

	int32 xi = x;
	const wchar_t *wcp = wstr;
	while( *wcp ) {
		uint32 code = *wcp;

		int fontW = fontTable->getCodeWidth(code);

		//cliping
		Rect drawRect ( xi, fontW + xi, y, fontH + y );
		Rect rect = drawRect.intersection( clipRect );
		rect.left -= xi;
		rect.right -= xi;
		rect.top -= y;
		rect.bottom -= y;
		SIGN_ASSERT( 0 <= rect.left );
		SIGN_ASSERT( 0 <= rect.top );

		if ( !rect.empty() ) {
			fontTable->putPattern( this, xi + rect.left, y + rect.top, rect.width(), rect.height(), rect.left, rect.top, fontW, fontH, fgColor, fgAlpha, bgColor, bgAlpha, code );
		}
		else {
			if ( (xi >= clipRect.right) || ( (xi+fontW) >= (int)maxx ) ) {
				break;
			}
		}
		
		if ( (xi+fontW) >= (int)maxx ) {
			break;
		}
		xi += fontW;

		++wcp;
	}
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, uint32 bgColor, const Rect &clipRect )
{
	drawString( x, y, w, h, wstr, fgColor, 1.0, bgColor, 1.0, clipRect );
}

//--------------------------------------------------------
void 
GraphicInfo::drawString(int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect )
{
	fontTable->setFontSize( w, h );
	drawString( x, y, wstr, fgColor, fgAlpha, bgColor, bgAlpha, clipRect );
}



//--------------------------------------------------------
//! �����񂪕`�悳�ꂽ���̕����s�N�Z�����ŕԂ�
/*
	@param[in] str			�`�悷�镶����
	@param[in] font_size	�����̃T�C�Y(�s�N�Z��)
	@return					�����񂪕`�悳�ꂽ���̕�(�s�N�Z��)
*/
int 
GraphicInfo::measureString( const char *str, int32 font_size )
{
	int total_width = 0;
	int old_width  = fontTable->getWidth();
	int old_height = fontTable->getHeight();
	fontTable->setFontSize( font_size, font_size );

	int32 wi;
	const char *cp = str;
	while( *cp ) {
		uint32 code;
		int len = getShiftJisCode( cp, &code );
		cp += len;

		wi = fontTable->getCodeWidth(code);
		total_width += wi;
	}

	fontTable->setFontSize( old_width, old_height );

	return total_width;
}

//--------------------------------------------------------
//wchar_t version
// code is unicode
int 
GraphicInfo::measureString( const wchar_t *wstr, int32 font_size )
{
	int total_width = 0;
	int old_width  = fontTable->getWidth();
	int old_height = fontTable->getHeight();
	fontTable->setFontSize( font_size, font_size );

	int32 wi;
	const wchar_t *wcp = wstr;
	while( *wcp ) {
		uint32 code = *wcp;
		wi = fontTable->getCodeWidth(code);
		total_width += wi;
		++wcp;
	}

	fontTable->setFontSize( old_width, old_height );

	return total_width;
}


//--------------------------------------------------------
bool 
GraphicInfo::isValidCharCode( uint32 code )
{
	return fontTable->isValidCharCode( code );
}



/*
//--------------------------------------------------------
void
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, uint32 bgColor)
{
	const char *cp = str;
	int32 xi;
	int32 wi, hi;

	xi = x;
	hi = fontTable->getHeight();
	while( *cp ) {
		uint32 code;
		if ( isShiftJisMultiByteChar(*cp) ) {
			code = ( (unsigned char)cp[1] | ( (unsigned char)cp[0] << 8) );
			cp += 2;
		}
		else {
			code = (unsigned char)cp[0];
			++cp;
		}
				
#ifdef USE_GNU_FONT
		if ( code <= 0x7F ) { //Ascii
			wi = fontTable->getCodeWidth( code );
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(code), fgColor, bgColor );
		}
		else if ( code <= 0x100 ) {//HalfWidth Katakana
			code = (code-0x40) | 0xFF00;
			wi = fontTable->getCodeWidth( code );
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(code), fgColor, bgColor );

		}
		else { //JisX0208 Kanji
			unsigned int jiscode;
			unsigned short unicode;
			jiscode = sjis2jis(code);
			unicode = Jisx0208ToUnicode11(jiscode);
			
			wi = fontTable->getCodeWidth(unicode);
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(unicode), fgColor, bgColor );
		} 
		// fontTable ga pattern wo katita houga yoi to omou
		wi = fontTable->getCodeWidth(code);
		putBitPattern(xi, y, wi, hi, fontTable->getPattern(code), fgColor, bgColor);
#endif
		xi += wi;
	}
}


//--------------------------------------------------------
void
GraphicInfo::drawString(int32 x, int32 y, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	const char *cp = str;
	int32 xi;
	int32 wi, hi;

	xi = x;
	hi = fontTable->getHeight();
	while( *cp ) {
		uint32 code;
		if ( isShiftJisMultiByteChar(*cp) ) {
			code = ( (unsigned char)cp[1] | ( (unsigned char)cp[0] << 8) );
			cp += 2;
		}
		else {
			code = (unsigned char)cp[0];
			++cp;
		}
				
#ifdef USE_GNU_FONT
		if ( code <= 0x7F ) { //Ascii
			wi = fontTable->getCodeWidth( code );
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(code), fgColor, bgColor );
		}
		else if ( code <= 0x100 ) {//HalfWidth Katakana
			code = (code-0x40) | 0xFF00;
			wi = fontTable->getCodeWidth( code );
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(code), fgColor, bgColor );

		}
		else { //JisX0208 Kanji
			unsigned int jiscode;
			unsigned short unicode;
			jiscode = sjis2jis(code);
			unicode = Jisx0208ToUnicode11(jiscode);
			
			wi = fontTable->getCodeWidth(unicode);
			putBitPattern( xi, y, wi, hi, fontTable->getPattern(unicode), fgColor, bgColor );
		} 
		// fontTable �� pattern ���������ق��������Ǝv��
#else
		wi = fontTable->getCodeWidth(code);
		putBitPattern(xi, y, wi, hi, fontTable->getPattern(code), fgColor, fgAlpha, bgColor, bgAlpha);
#endif
		xi += wi;
	}
}
*/

//--------------------------------------------------------
const char *
GraphicInfo::setFont(const char *fontName)
{
	FontTable *oldFontTable;
	oldFontTable = fontTable;

	fontTable = fontManager->lookupFontTable(fontName);
	return oldFontTable->getFontName();
}



} // namespace sign
} // namespace sgl

