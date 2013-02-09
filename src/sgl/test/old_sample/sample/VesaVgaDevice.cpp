#include <cstdlib>
#include <cstdio> // for perror
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "sign/sgl++/DevicePixel.h"
#include "VesaVgaDevice.h"

namespace sign {
namespace sgl {

const char *VesaVgaDevice::deviceName = "linux fb";

//----------------------------------------------------------------------------
VesaVgaDevice::VesaVgaDevice(pixel_type *frame) :
	FrameBufferDevice<DevicePixel_A8R8G8B8>(
		deviceName, ZERO_P_LEFT_UP, ANGLE_R000,
		STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y,
		MAX_SIZE_X, MAX_SIZE_Y, 0, 0, MAX_SIZE_X, MAX_SIZE_Y,
		frame, false), fds(-1)
{
}

//----------------------------------------------------------------------------
VesaVgaDevice::~VesaVgaDevice()
{
	if ( fds != -1 ) {
		close(fds);
	}
}

//----------------------------------------------------------------------------
VesaVgaDevice *VesaVgaDevice::createDevice( const char *device_name )
{
	int fds = -1;
	VesaVgaDevice *vesa = 0;
	try {
		fds = open( device_name , O_RDWR );
		if ( fds < 0 ) {
			throw device_name;
		}
		void *fb;
		int screensize;
		screensize = VesaVgaDevice::MAX_SIZE_X * VesaVgaDevice::MAX_SIZE_Y * 4;
		fb = mmap(0, screensize, PROT_READ | PROT_WRITE,MAP_SHARED, fds, 0);
		if ( fb == (void *)-1 ) {
			throw device_name;
		}
		vesa = new VesaVgaDevice((pixel_type *)fb);
		vesa->fds = fds;
	} catch ( const char *error_str ) {
		perror(error_str);
		if ( vesa ) {
			delete vesa;	
			fds = -1;
		}
		if ( fds != -1 ) {
			close(fds);
		}
		return 0;
	}
	
	return vesa;
}


void *VesaVgaDevice::getFrameBuffer()
{
	return buf;
}

//----------------------------------------------------------------------------

}; // namespace sgl
}; // namespace sign
