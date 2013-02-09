#include <cstdlib>
#include <cstdio> // for perror
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "sign/sgl++/DevicePixel.h"
#include "Armadillo500FXLCDDevice.h"

namespace sign {
namespace sgl {

const char *Armadillo500FXLCDDevice::deviceName = "linux fb";

//----------------------------------------------------------------------------
Armadillo500FXLCDDevice::Armadillo500FXLCDDevice(pixel_type *frame) :
	FrameBufferDevice<DevicePixel_R5G6B5>(
		deviceName, ZERO_P_LEFT_UP, ANGLE_R000,
		STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y,
		MAX_SIZE_X, MAX_SIZE_Y, 0, 0, MAX_SIZE_X, MAX_SIZE_Y, frame, false), fds(-1)
{
}

//----------------------------------------------------------------------------
Armadillo500FXLCDDevice::~Armadillo500FXLCDDevice()
{
	if ( fds != -1 ) {
		close(fds);
	}
}

//----------------------------------------------------------------------------
Armadillo500FXLCDDevice *Armadillo500FXLCDDevice::createDevice(const char *device_name )
{
	int fds = -1;
	Armadillo500FXLCDDevice *lcdDevice = 0;
	try {
		fds = open( device_name , O_RDWR);
		if ( fds < 0 ) {
			throw device_name;
		}
		void *fb;
		int screensize;
		screensize = Armadillo500FXLCDDevice::MAX_SIZE_X * Armadillo500FXLCDDevice::MAX_SIZE_Y * 2;
		fb = mmap(0, screensize, PROT_READ | PROT_WRITE,MAP_SHARED, fds, 0);
		if ( fb == (void *)-1 ) {
			throw device_name;
		}
		lcdDevice = new Armadillo500FXLCDDevice((pixel_type *)fb);
		lcdDevice->fds = fds;
	} catch ( const char *error_str ) {
		perror(error_str);
		if ( lcdDevice ) {
			delete lcdDevice;	
			fds = -1;
		}
		if ( fds != -1 ) {
			close(fds);
		}
		return 0;
	}
	
	return lcdDevice;
}


void *Armadillo500FXLCDDevice::getFrameBuffer()
{
	return buf;
}

//----------------------------------------------------------------------------

}; // namespace sgl
}; // namespace sign
