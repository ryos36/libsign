/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_GRAPHIC_DEVICE_MANAGER_H__
#define __SIGN_GRAPHIC_DEVICE_MANAGER_H__

#include "sgl.h"

namespace sign {
namespace sgl {

class DefaultGraphicDeviceManager : public GraphicDeviceManager {
public:
	DefaultGraphicDeviceManager(void);
	virtual ~DefaultGraphicDeviceManager(void);

	GraphicDevice *setDevice(GraphicDevice *device);
	GraphicDevice *lookupDevice(const char *deviceName);
protected:
	GraphicDevice *device0;
	GraphicDevice *device1;
};

} // namespace sgo
} // namespace sign

#endif /* __SIGN_GRAPHIC_DEVICE_MANAGER_H__ */
