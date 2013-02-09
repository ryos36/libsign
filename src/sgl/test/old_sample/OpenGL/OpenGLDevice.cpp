/* Copyright (c) 2004, 2007-2008 Sinby Corporation, All Rights Reserved */

#include <cwchar>
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"
#include "sign/sgl++/OpenGLDevice.h"
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

//-----------------------------------------------------
const char 
OpenGLDevice::OPEN_GL_DEVICE_NAME[] = "openGL";

//-----------------------------------------------------
OpenGLDevice::OpenGLDevice(const char *deviceName, int32 maxSizeX, int32 maxSizeY, int &argc, char **argv) 
: FrameBufferDevice<DEVICE_PIXEL>(deviceName, ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y , 
					maxSizeX, maxSizeY, 0, 0, maxSizeX, maxSizeY, NULL, true )
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

	dstbuf = new unsigned char[ 3 * theDevice->maxSizeY * theDevice->maxSizeX ];
}

//-----------------------------------------------------
OpenGLDevice::~OpenGLDevice(void)
{
	if (dstbuf) {
		delete [] dstbuf;
	}
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
void
OpenGLDevice::display(void)
{
#ifdef CLEAR_SCREEN
	glClear( GL_COLOR_BUFFER_BIT );
#endif
	
	FrameBufferDevice<DEVICE_PIXEL>::pixel_type *p;
	p = theDevice->buf;

	unsigned char *dst = dstbuf;
	uint32 r, g, b;
	for( int y = theDevice->maxSizeY-1 ; y >= 0; --y ) {
		dst = &dstbuf[ y * theDevice->maxSizeX * 3 ];
		for( uint32 x = 0 ; x < theDevice->maxSizeX; ++x ) {
			device_pixel_type::unpack( *p, r, g, b );
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

