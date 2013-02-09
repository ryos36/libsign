#ifndef __VESA_VGA_DEVICE_H__
#define __VESA_VGA_DEVICE_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"

namespace sign {
namespace sgl {

class VesaVgaDevice : public FrameBufferDevice<DevicePixel_A8R8G8B8> {
public:
	typedef DevicePixel_A8R8G8B8::pixel_type pixel_type;
	enum {
		MAX_SIZE_X      = 640,
		MAX_SIZE_Y      = 480,
	};
	VesaVgaDevice( pixel_type *frame );
	~VesaVgaDevice(void);

	void *getFrameBuffer();
public:
	static const char *deviceName;
	static VesaVgaDevice *createDevice(const char *device_name = "/dev/fb0" );
	
public:
	void repaint(void) {}
	void backlight(bool on) {}

private:
	int fds;
};

}; // namespace sgl
}; // namespace sign

#endif /* __VESA_VGA_DEVICE_H__ */
