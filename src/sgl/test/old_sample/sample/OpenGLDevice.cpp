/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */


#include "OpenGLDevice.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"
#include "sign/sgl++/OpenGLUtils.h"

#if defined(__linux__)
#include <unistd.h> // for usleep
#define SLEEP(__ms__) usleep(__ms__*1000)
#elif defined(_MSC_VER)
#include <windows.h>
#define SLEEP(__ms__) Sleep(__ms__)
#else
#define SLEEP(__ms__)
#endif


namespace sign {
namespace sgl {

#define DEVICE_PIXEL DevicePixel_A8B8G8R8

//FrameBufferDevice.h ‚ð‰B•Á‚·‚é‚½‚ß‚Ì“à•”ŽÀ‘•ƒNƒ‰ƒX
class OpenGLDeviceImpl : public FrameBufferDevice<DEVICE_PIXEL> {
public:
	OpenGLDeviceImpl(const char *deviceName, int32 maxSizeX, int32 maxSizeY);
	~OpenGLDeviceImpl(void);

public:
	void repaint(void) {}
	void backlight(bool on) {}
	static void display(void) {}
private:

};

//-----------------------------------------------------
OpenGLDevice::OpenGLDevice(const char *deviceName, int32 maxSizeX, int32 maxSizeY, int &argc, char **argv) 
: GraphicDevice(deviceName, ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y , 
					maxSizeX, maxSizeY, 0, 0, maxSizeX, maxSizeY )
{
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA );

	glutInitWindowSize(maxSizeX, maxSizeY);
	glutCreateWindow(deviceName);

	glViewport(0, 0, maxSizeX, maxSizeY);
	glLoadIdentity();
	//glOrtho(-1.0, 128.0, -128.0, 1.0, -1.0, 1.0);
	glOrtho(-1.0, (double)maxSizeX, (double)-maxSizeY, 1.0, -1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glRotated(180.0, 0.0, 0.0, 1.0);
	glRotated(180.0, 0.0, 1.0, 0.0);

	glutDisplayFunc(display);
	glutIdleFunc(display);
	theDevice = this;

	dstbuf = new unsigned char[ 3 * maxSizeY * maxSizeX ];
	implDevice = new OpenGLDeviceImpl("OpenGLDeviceImpl", maxSizeX, maxSizeY);
}

//-----------------------------------------------------
OpenGLDevice::~OpenGLDevice(void)
{
	// nothing to do
	if (dstbuf) {
		delete [] dstbuf;
	}
	delete implDevice;
}

//-----------------------------------------------------
void 
OpenGLDevice::setPixel(int32 x, int32 y, uint32 color)
{
	implDevice->setPixel(x, y, color);
}

//-----------------------------------------------------
uint32 
OpenGLDevice::getPixel(int32 x, int32 y)
{
	return implDevice->getPixel(x, y);
}

//-----------------------------------------------------
void 
OpenGLDevice::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color)
{
	implDevice->drawLine(x1, y1, x2, y2, color);
}

//-----------------------------------------------------
void 
OpenGLDevice::drawLine(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, double alpha )
{
	implDevice->drawLine(x1, y1, x2, y2, color, alpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color)
{
	implDevice->fillRectangle(x, y, w, h, color);
}

//-----------------------------------------------------
void 
OpenGLDevice::fillRectangle(int32 x, int32 y, uint32 w, uint32 h, uint32 color, double alpha)
{
	implDevice->fillRectangle(x, y, w, h, color, alpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	implDevice->putPattern(x, y, w, h, buf);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, void *srcbuf, uint32 transparentColor)
{
	implDevice->putPattern(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, transparentColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double alpha)
{
	implDevice->putPattern(x, y, w, h, buf, alpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf, double *alphabuf)
{
	implDevice->putPattern(x, y, w, h, buf, alphabuf);
}

//-----------------------------------------------------
void 
OpenGLDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	implDevice->putBitPattern(x, y, w, h, buf, fgColor, bgColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	implDevice->putBitPattern(x, y, w, h, buf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, uint32 bgColor)
{
	implDevice->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, bgColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putBitPattern(int32 x, int32 y, uint32 w, uint32 h, uint32 scaleX, uint32 scaleY, const uint8 *buf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha)
{
	implDevice->putBitPattern(x, y, w, h, scaleX, scaleY, buf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern8bpp(int32 x, int32 y, uint32 w, uint32 h, const uint8 *putbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	implDevice->putPattern8bpp(x, y, w, h, putbuf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern8bpp(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 fgColor, double fgAlpha, uint32 bgColor, double bgAlpha )
{
	implDevice->putPattern8bpp(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, fgColor, fgAlpha, bgColor, bgAlpha);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern8bppWithPalette( int32 x, int32 y, uint32 w, uint32 h, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	implDevice->putPattern8bppWithPalette(x, y, w, h, srcbuf, palette, transparentColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern8bppWithPalette( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint8 *srcbuf, uint32 *palette, uint32 transparentColor )
{
	implDevice->putPattern8bppWithPalette(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, srcbuf, palette, transparentColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern32bpp( int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf )
{
	implDevice->putPattern32bpp(x, y, w, h, buf);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern32bpp( int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcX, int32 srcY, uint32 srcW, uint32 srcH, const uint32 *buf, uint32 transparentColor )
{
	implDevice->putPattern32bpp(dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, buf, transparentColor);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern32bppWithAlpha(int32 x, int32 y, uint32 w, uint32 h, const uint32 *buf )
{
	implDevice->putPattern32bppWithAlpha(x, y, w, h, buf);
}

//-----------------------------------------------------
void 
OpenGLDevice::putPattern32bppWithAlpha(int32 dstX, int32 dstY, uint32 dstW, uint32 dstH, int32 srcOffsetX, int32 srcOffsetY, uint32 srcW, uint32 srcH, const uint32 *buf )
{
	implDevice->putPattern32bppWithAlpha(dstX, dstY, dstW, dstH, srcOffsetX, srcOffsetY, srcW, srcH, buf);
}

//-----------------------------------------------------
void 
OpenGLDevice::getPattern(int32 x, int32 y, uint32 w, uint32 h, void *buf)
{
	implDevice->getPattern(x, y, w, h, buf);
}

//-----------------------------------------------------
void
OpenGLDevice::repaint(void)
{
	// nothing to do
}

//-----------------------------------------------------
void
OpenGLDevice::backlight(bool on)
{
	// nothing to do
}


//-----------------------------------------------------
const void *
OpenGLDevice::getBuffer()
{
	OpenGLDeviceImpl *device = static_cast<OpenGLDeviceImpl*>(implDevice);
	return device->getBuffer();
}

//-----------------------------------------------------
void
OpenGLDevice::display(void)
{
#ifdef CLEAR_SCREEN
	glClear( GL_COLOR_BUFFER_BIT );
#endif
	FrameBufferDevice<DEVICE_PIXEL>::pixel_type *p;
	p = (DEVICE_PIXEL::pixel_type*)(theDevice->getBuffer());

	unsigned char *dst = dstbuf;
	uint32 r, g, b;
	for( int y = theDevice->maxSizeY-1 ; y >= 0; --y ) {
		dst = &dstbuf[ y * theDevice->maxSizeX * 3 ];
		for( uint32 x = 0 ; x < theDevice->maxSizeX; ++x ) {
			DEVICE_PIXEL::unpack( *p, r, g, b );
			*dst++ = r;
			*dst++ = g;
			*dst++ = b;
						
			++p;
		}
	}

	glDrawPixels( theDevice->maxSizeX, theDevice->maxSizeY, GL_RGB, GL_UNSIGNED_BYTE, dstbuf );

	glFlush();
}



//-----------------------------------------------------
OpenGLDeviceImpl::OpenGLDeviceImpl(const char *deviceName, int32 maxSizeX, int32 maxSizeY)
: FrameBufferDevice<DEVICE_PIXEL>(deviceName, ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y , 
					maxSizeX, maxSizeY, 0, 0, maxSizeX, maxSizeY, NULL, true )
{
}


//-----------------------------------------------------
OpenGLDeviceImpl::~OpenGLDeviceImpl(void)
{
}




//-----------------------------------------------------
unsigned char *OpenGLDevice::dstbuf;
OpenGLDevice* OpenGLDevice::theDevice;
OpenGLEventManager OpenGLDevice::theOpenGLEventManager;

//-----------------------------------------------------
OpenGLEventManager::OpenGLEventManager(void)
{
	// nothing to do
}

//-----------------------------------------------------
OpenGLEventManager::~OpenGLEventManager(void)
{
	// nothing to do
}

//-----------------------------------------------------
void
OpenGLEventManager::setKeyboardListener(KeyboardListener *listener)
{
	EventManager::setKeyboardListener(listener);
	glutKeyboardFunc(openGL_KeyFunc);
}

//-----------------------------------------------------
void
OpenGLEventManager::setPointingDeviceListener(PointingDeviceListener *listener)
{
	EventManager::setPointingDeviceListener(listener);
	glutMouseFunc(openGL_MouseFunc);
}

//-----------------------------------------------------
int32
OpenGLEventManager::getModifiers(void)
{
	return openGL_getModifiers();
}

//-----------------------------------------------------
void
OpenGLEventManager::doEventLoop(void)
{
	glutMainLoop();
}

//-----------------------------------------------------
void
OpenGLEventManager::msleep(unsigned int miliseconds)
{
	SLEEP(miliseconds);
}

//-----------------------------------------------------
void
OpenGLEventManager::openGL_KeyFunc(unsigned char key, int x, int y)
{
	if ( OpenGLDevice::theOpenGLEventManager.keyListener ) {
		int32 status;
		status = openGL_getModifiers();
		OpenGLDevice::theOpenGLEventManager.keyListener->keyPressed((char)key, status);
	}
}

//-----------------------------------------------------
void
OpenGLEventManager::openGL_MouseFunc(int button, int state, int x, int y)
{
	if ( OpenGLDevice::theOpenGLEventManager.pointingDeviceListener ) {
		uint32 sglButton;
		uint32 sglState;
		sglButton = 0;
		sglState = 0;
		switch ( button ) {
		case GLUT_LEFT_BUTTON:
			sglButton = PointingDeviceListener::LEFT_BUTTON;
			break;
		case GLUT_MIDDLE_BUTTON:
			sglButton = PointingDeviceListener::MIDDLE_BUTTON;
			break;
		case GLUT_RIGHT_BUTTON:
			sglButton = PointingDeviceListener::RIGHT_BUTTON;
			break;
		}
		switch ( state ) {
		case GLUT_UP:
			sglState = PointingDeviceListener::POINTING_DEVICE_UP;
			break;
		case GLUT_DOWN:
			sglState = PointingDeviceListener::POINTING_DEVICE_DOWN;
			break;
		}
		OpenGLDevice::theOpenGLEventManager.pointingDeviceListener->changed(sglButton, sglState, x, y);
	}
}

//-----------------------------------------------------
int32 
OpenGLEventManager::openGL_getModifiers(void)
{
	int mod;
	uint32 status;

	mod = glutGetModifiers();
	status = 0;
	if ( mod & GLUT_ACTIVE_SHIFT ) {
		status |= KeyboardListener::SHIFT;
	}
	if ( mod & GLUT_ACTIVE_CTRL ) {
		status |= KeyboardListener::CTRL;
	}
	if ( mod & GLUT_ACTIVE_ALT ) {
		status |= KeyboardListener::ALT;
	}
	return status;
}

} // namespace sign
} // namespace sgl

