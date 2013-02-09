#ifndef __ARMADILLO_500_FX_LCD_DEVICE_H__
#define __ARMADILLO_500_FX_LCD_DEVICE_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"

namespace sign {
namespace sgl {

class Armadillo500FXLCDDevice : public FrameBufferDevice<DevicePixel_R5G6B5> {
public:
	typedef DevicePixel_R5G6B5::pixel_type pixel_type;
	enum {
		MAX_SIZE_X      = 640,
		MAX_SIZE_Y      = 480,
	};
	Armadillo500FXLCDDevice( pixel_type *frame );
	~Armadillo500FXLCDDevice(void);

	
	void *getFrameBuffer();
public:
	static const char *deviceName;
	static Armadillo500FXLCDDevice *createDevice(const char *device_name = "/dev/fb0" );
	
public:
	void repaint(void) {}
	void backlight(bool on) {}
private:
	int fds;
};

}; // namespace sgl
}; // namespace sign

#endif /* __ARMADILLO_500_FX_LCD_DEVICE_H__ */
