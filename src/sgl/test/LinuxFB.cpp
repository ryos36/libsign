#include <cstdlib>
#include <cstdio> // for perror
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include "sign/sgl++/DevicePixel.h"
#include "LinuxFB.h"

#include <iostream>

using namespace sign::sgl;

//----------------------------------------------------------------------------
LinuxFB::LinuxFB(const char *deviceName, pixel_type *frame) :
	FrameBufferDevice<DevicePixel_B8G8R8A8>(
		deviceName, ZERO_P_LEFT_UP, ANGLE_R000,
		STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y,
		2048, 2048, 0, 0, 2048, 2048, frame, false), fds(-1)
{
}

//----------------------------------------------------------------------------
LinuxFB::~LinuxFB()
{
	if ( fds != -1 ) {
		close(fds);
	}
}

//----------------------------------------------------------------------------
LinuxFB *
LinuxFB::createLCDDevice(const char *device_name)
{
	int fds = -1;
	LinuxFB *lcdDevice = 0;
	try {
		struct fb_var_screeninfo vinfo;
		struct fb_fix_screeninfo finfo;

		int rv;
		fds = open( "/dev/fb1" , O_RDWR);
		if ( fds < 0 ) {
			throw device_name;
		}

		rv = ioctl(fds, FBIOGET_FSCREENINFO, &finfo);
		if ( rv < 0 ) {
			throw device_name;
		}

		rv = ioctl(fds, FBIOGET_VSCREENINFO, &vinfo);
		if ( rv < 0 ) {
			throw device_name;
		}

		void *fb;
		int screensize;
		screensize = 2048 * 2048 * 32 / 8 ;
		/*
		xres = vinfo.xres ;
		yres = vinfo.yres ;
		vbpp = vinfo.bits_per_pixel ;
		*/

		fb = mmap(0, screensize, PROT_READ | PROT_WRITE,MAP_SHARED, fds, 0);
		if ( fb == (void *)-1 ) {
			throw device_name;
		}
		lcdDevice = new LinuxFB(device_name, (pixel_type *)fb);
		lcdDevice->fds = fds;
	} catch ( const char *error_str ) {
		perror(error_str);
		if ( lcdDevice ) {
			delete lcdDevice;	
		}
	}

	if ( fds != -1 ) {
		close(fds);
	}
	
	return lcdDevice;
}

//----------------------------------------------------------------------------

void *
LinuxFB::getFrameBuffer()
{
	return buf;
}


