#ifndef __LINUX_FB_H__
#define __LINUX_FB_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"

#include <linux/fb.h>

using namespace sign::sgl;

class LinuxFB : public FrameBufferDevice<DevicePixel_B8G8R8A8> {
public:
	typedef DevicePixel_B8G8R8A8::pixel_type pixel_type;
	LinuxFB( const char *deviceName, pixel_type *frame );
	virtual ~LinuxFB(void);
	
	void *getFrameBuffer();
public:
	void repaint(void) {}
	void backlight(bool on) {}

private:
	int fds;

public:
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	static LinuxFB *createLCDDevice(const char *device_name);
};

#endif /* __LINUX_FB_H__ */
