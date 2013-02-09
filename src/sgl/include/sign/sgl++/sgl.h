/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_H__
#define __SIGN_SGL_H__

#ifndef _GCC_WCHAR_T
#ifndef __cplusplus
#include <wchar.h>
#endif
#endif
#include "sign/sign_types.h"
#include "sign/sgl++/primitives.h"

#ifdef _MSC_VER
#include <windows.h> //criticalsection
#endif

namespace sign {
namespace sgl {

//!* deprecated
bool isShiftJisMultiByteChar( const char c );

// 
bool isZenkakuKatakana( uint32 code );
bool isZenkakuHiragana( uint32 code );

//---------------------------------------------------------------
class GraphicFunction {
public:
	enum {
		ZERO_P_LEFT_UP = 0x0000,
		ZERO_P_LEFT_DOWN = 0x0001,
		ZERO_P_RIGHT_UP = 0x0002,
		ZERO_P_RIGHT_DOWN = 0x0003,
	};

	enum {
		ANGLE_R000 = 0x0000,
		ANGLE_R090 = 0x0001,
		ANGLE_R180 = 0x0002,
		ANGLE_R270 = 0x0003,
	};

	enum {
		AREA_D_XINC = 0x0001,
		AREA_D_XDEC = 0x0000,

		AREA_D_YINC = 0x0002,
		AREA_D_YDEC = 0x0000,

		AREA_D_X2Y  = 0x0004,
		AREA_D_Y2X  = 0x0000,
	};

	enum {
		FONT_D_LtoR = 0x0000,
		FONT_D_RtoL = 0x0001,
		FONT_D_UtoD = 0x0002,
		FONT_D_DtoU = 0x0003,
	};


public:
	virtual ~GraphicFunction() {};

	virtual void setPixel(int32 x, int32 y, uint32 color) = 0;
	virtual uint32 getPixel(int32 x, int32 y) = 0;
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color) = 0;
	virtual void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha ) = 0;
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color) = 0;
	virtual void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha) = 0;

	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf) = 0;
	virtual void putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor ) = 0;
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha) = 0;
	virtual void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf) = 0;
		
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor) = 0;
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha) = 0;
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor) = 0;
	virtual void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha) = 0;
	
	virtual void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha ) = 0;
	virtual void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha ) = 0;
	virtual void putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor ) = 0;
	virtual void putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor ) = 0;
	virtual void putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf ) = 0;
	virtual void putPattern32bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor ) = 0;
	virtual void putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf ) = 0;
	virtual void putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf ) = 0;

	virtual void getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf) = 0;

	virtual void repaint(void) = 0;
	virtual void backlight(bool on) = 0;

};


//---------------------------------------------------------------
class GraphicDevice : public GraphicFunction {
public:
	enum {
		STATUS_HAS_XINC = 0x0001,
		STATUS_HAS_YINC = 0x0002,
		STATUS_HAS_XDEC = 0x0004,
		STATUS_HAS_YDEC = 0x0008,
		STATUS_HAS_X2Y  = 0x0010,
		STATUS_HAS_Y2X  = 0x0020
	};

	enum {
		STATUS_ZERO_P_MASK	= 0x0003,
		STATUS_ZERO_P_SHIFT	= 0,

		STATUS_ANGLE_MASK	= 0x000C,
		STATUS_ANGLE_SHIFT	= 2,

		STATUS_HAS_D_SPEC_MASK	= 0x03F0,
		STATUS_HAS_D_SPEC_SHIFT	= 4,

		STATUS_HAS_COLOR_TYPE_MASK     = 0xFC00,
		STATUS_HAS_COLOR_TYPE_SHIFT    = 10
	};
public:
	GraphicDevice(const char *deviceName, uint32 zero_postion, uint32 angle, uint32 direction_spec, uint32 maxSizeX, uint32 maxSizeY, int32 viewBaseX, int32 viewBaseY, uint32 viewSizeX, uint32 viewSizeY );
	virtual ~GraphicDevice() {}

	virtual bool setDeviceStatus(uint32 status);
	uint32 getMaxSizeX() const { return maxSizeX; }
	uint32 getMaxSizeY() const { return maxSizeY; }

	const char *deviceName;
protected:
	uint32 status;

	uint32 maxSizeX;
	uint32 maxSizeY;

	int32 viewBaseX;
	int32 viewBaseY;

	uint32 viewSizeX;
	uint32 viewSizeY;

};


//---------------------------------------------------------------
class KeyboardListener {
public:
	virtual void keyPressed(char key, int32 status) = 0;
	virtual ~KeyboardListener() {}

	enum {
		NONE = 0,
		SHIFT = 1,
		CTRL = 2,
		ALT = 4
	};
};


//---------------------------------------------------------------
class PointingDeviceListener {
public:
	virtual void changed(uint32 button, uint32 status, int32 x, int32 y) = 0;
	virtual ~PointingDeviceListener() {}

	enum {
		LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON
	};
	enum {
		POINTING_DEVICE_DOWN, POINTING_DEVICE_UP
	};
};


//---------------------------------------------------------------
class EventManager {
public:
	EventManager(void);
	virtual ~EventManager(void) {}
public:
	virtual void setKeyboardListener(KeyboardListener *listener);
	virtual void setPointingDeviceListener(PointingDeviceListener *listener);
	virtual int32 getModifiers(void) = 0;
	virtual void doEventLoop(void) = 0;
	virtual void msleep(unsigned int miliseconds) = 0;

protected:
	KeyboardListener *keyListener;
	PointingDeviceListener *pointingDeviceListener;
};

class GraphicDeviceManager {
public:
	virtual ~GraphicDeviceManager() {}

	virtual GraphicDevice *setDevice(GraphicDevice *device) = 0;
	virtual GraphicDevice *lookupDevice(const char *deviceName) = 0;
};


class FontTable;
class FontManager;
//---------------------------------------------------------------
class GraphicInfo : public GraphicFunction, public EventManager {
public:
	GraphicInfo(const char *deviceName );
	~GraphicInfo(void);

public:
	/* method as EventManager */
	void setKeyboardListener(KeyboardListener *listener);
	void setPointingDeviceListener(PointingDeviceListener *listener);
	int32 getModifiers(void);
	void doEventLoop(void);
	void msleep(unsigned int miliseconds);

public:
	static void setGraphicDeviceManager(GraphicDeviceManager *manager);
	static void setFontManager(FontManager *fontManager);
	static void setEventManager(EventManager *eventManager);

protected:
	static GraphicDeviceManager *manager;
	static FontManager *fontManager;
	static EventManager *eventManager;

protected:
	FontTable *fontTable;
	GraphicDevice *device;

public:
	inline void setPixel(int32 x, int32 y, uint32 color);
	inline uint32 getPixel(int32 x, int32 y);
	inline void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color );
	inline void drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha );
	inline void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color);
	inline void fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha);

	inline void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);
	inline void putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor );
	inline void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha);
	inline void putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf);
	
	inline void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	inline void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);
	inline void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor);
	inline void putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);

	inline void putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	inline void putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	inline void putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );
	inline void putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );
	
	inline void putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	inline void putPattern32bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor );
	inline void putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	inline void putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf );

	inline void getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);

	inline void repaint(void);
	inline void backlight(bool on);

	/* string */
	void drawString(int32 x, int32 y, const char *str, uint32 fgColor, uint32 bgColor );
	void drawString(int32 x, int32 y, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);
	void drawString( int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, uint32 bgColor );
	void drawString( int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );

	//with cliping
	void drawString(int32 x, int32 y, const char *str, uint32 fgColor, uint32 bgColor, const Rect &clipRect );
	void drawString(int32 x, int32 y, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect );
	void drawString(int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, uint32 bgColor, const Rect &clipRect );
	void drawString(int32 x, int32 y, uint32 w, uint32 h, const char *str, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect );

	//wchar_t version
	void drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, uint32 bgColor);
	void drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha);
	void drawString( int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, uint32 bgColor );
	void drawString( int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha );
	
	//with cliping wchar_t version
	void drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, uint32 bgColor, const Rect &clipRect );
	void drawString(int32 x, int32 y, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect );
	void drawString(int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, uint32 bgColor, const Rect &clipRect );
	void drawString(int32 x, int32 y, uint32 w, uint32 h, const wchar_t *wstr, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha, const Rect &clipRect );


	int measureString( const char *str, int32 font_width );
	int measureString( const wchar_t *wstr, int32 font_width );

	bool isValidCharCode( uint32 code );

	const char* setFont(const char *fontName);

	/* lock for device */
	virtual void lock(void);
	virtual void unlock(void);

#ifdef _MSC_VER
private:
	CRITICAL_SECTION cs;
#endif
};

} // namespace sgl
} // namespace sign

#include "sign/sgl++/sgl.inc"

#endif /* __SIGN_SGL_H__ */
