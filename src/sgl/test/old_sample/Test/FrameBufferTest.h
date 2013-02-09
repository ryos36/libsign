#include "sign/sgl++/FrameBufferDevice.h"

class MemoryDevice
	: public sign::sgl::FrameBufferDevice< sign::sgl::DevicePixel_A8B8G8R8 >
{
public:
	MemoryDevice(const char *name, int maxSizeX, int maxSizeY )
	: FrameBufferDevice< sign::sgl::DevicePixel_A8B8G8R8 > ( name, ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y , maxSizeX, maxSizeY, 0, 0, maxSizeX, maxSizeY, 0, true )
	{}

	virtual void repaint(void){};
	virtual void backlight(bool on){};
};
