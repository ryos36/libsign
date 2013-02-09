#ifndef __SIGN_SGL_INIT_H__
#define __SIGN_SGL_INIT_H__

namespace sign {
namespace sgl {

class GraphicDevice;
class GraphicDeviceManager;
class FontTable;
class FontManager;
class EventManager;
class KeyboardListener;
class PointingDeviceListener;
class GraphicInfo;

GraphicInfo *
initGraphicInfo( GraphicDevice *device,
                 GraphicDeviceManager *deviceManager = 0,
                 FontTable *fontTable = 0,
                 FontManager *fontManager = 0,
				 EventManager *eventManager = 0,
                 KeyboardListener *keyListener = 0,
				 PointingDeviceListener *pointingDeviceListener = 0);
}
}

#endif //__SIGN_SGL_INIT_H__
