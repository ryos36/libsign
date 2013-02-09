/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_OPEN_GL_DEVICE_H__
#define __SIGN_SGL_OPEN_GL_DEVICE_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/DevicePixel.h"

namespace sign {
namespace sgl {

class OpenGLEventManager : public EventManager {
public:
	OpenGLEventManager(void);
	~OpenGLEventManager(void);
public:
	void setKeyboardListener(KeyboardListener *listener);
	void setPointingDeviceListener(PointingDeviceListener *listener);
	int32 getModifiers(void);
	void doEventLoop(void);
	void msleep(unsigned int miliseconds);

private:
	static void openGL_KeyFunc(unsigned char key, int x, int y); 
	static void openGL_MouseFunc(int button, int state, int x, int y); 
	static int32 openGL_getModifiers(void);
};


class OpenGLDevice : public GraphicDevice {
public:
	// OpenGLDevice を DevicePixel から導出せずに抽象化しているので、
	// 以下の typedef は、本来ここに記述すべきではないが...
	typedef DevicePixel_A8B8G8R8 device_pixel_type;
	typedef device_pixel_type::pixel_type pixel_type;

	OpenGLDevice(const char *deviceName, int32 maxSizeX, int32 maxSizeY, int &argc, char **argv);
	~OpenGLDevice(void);

public:
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
	virtual void putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );
	virtual void putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor );

	virtual void putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor );
	virtual void putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf );
	virtual void putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf );

	virtual void getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf);

	virtual void repaint(void);
	virtual void backlight(bool on);

	const void* getBuffer();
	static void display(void);
	static OpenGLDevice *theDevice;

	static OpenGLEventManager theOpenGLEventManager;
private:
	static unsigned char *dstbuf;
	GraphicDevice *implDevice;
};

}; // namespace sgl
}; // namespace sign

#endif /* __SIGN_SGL_OPEN_GL_DEVICE_H__ */
