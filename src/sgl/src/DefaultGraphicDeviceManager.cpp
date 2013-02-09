/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#include <string.h>
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "string.h" //strcmp

namespace sign {
namespace sgl {

//-----------------------------------------------------
DefaultGraphicDeviceManager::DefaultGraphicDeviceManager(void)
	: device0(0), device1(0)
{
	// nothing to do
}

//-----------------------------------------------------
DefaultGraphicDeviceManager::~DefaultGraphicDeviceManager(void)
{
	// nothing to do
}

//-----------------------------------------------------
GraphicDevice *
DefaultGraphicDeviceManager::setDevice(GraphicDevice *device)
{
	if ( device0 == 0 ) {
		device0 = device;	
	} else {
		device1 = device;	
	}

	return 0;
}

//-----------------------------------------------------
GraphicDevice *
DefaultGraphicDeviceManager::lookupDevice(const char *deviceName)
{
	if ( device0 == 0 ) {
		return 0;
	}
	if ( strcmp(device0->deviceName, deviceName) == 0 ) {
		return device0;
	}

	if ( device1 == 0 ) {
		return 0;
	}
	if ( strcmp(device1->deviceName, deviceName) == 0 ) {
		return device1;
	}

	return 0;
}

} // namespace sign
} // namespace sgl
