/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_OPEN_GL_DEVICE_H__
#define __SIGN_SGL_OPEN_GL_DEVICE_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
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

#define DEVICE_PIXEL DevicePixel_A8B8G8R8

class OpenGLDevice : public FrameBufferDevice<DEVICE_PIXEL> {
public:
	OpenGLDevice(const char *deviceName, int32 maxSizeX, int32 maxSizeY, int &argc, char **argv);
	~OpenGLDevice(void);

public:
	void repaint(void);
	void backlight(bool on);

	static void display(void);
	static OpenGLDevice *theDevice;

	static OpenGLEventManager theOpenGLEventManager;
	static const char OPEN_GL_DEVICE_NAME[];
private:
	static unsigned char *dstbuf;
};

}; // namespace sgl
}; // namespace sign

#endif /* __SIGN_SGL_OPEN_GL_DEVICE_H__ */
