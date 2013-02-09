
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/gnu_unicode.h"
#include "sgl_init.h"


namespace sign {
namespace sgl {

//-------------------------------------------------------------
GraphicInfo *
initGraphicInfo( GraphicDevice *device,
                 GraphicDeviceManager *deviceManager,
                 FontTable *fontTable,
                 FontManager *fontManager,
				 EventManager *eventManager,
                 KeyboardListener *keyListener,
				 PointingDeviceListener *pointingDeviceListener
				 )
{
	//init graphic device
	if ( deviceManager == 0 ) {
		static DefaultGraphicDeviceManager s_deviceManager;
		deviceManager = &s_deviceManager;
	}
	deviceManager->setDevice(device);
	GraphicInfo::setGraphicDeviceManager( deviceManager );

	//init font
	if ( fontManager == 0 ) {
		static DefaultFontManager s_fontManager;
		fontManager = &s_fontManager;
	}
	if ( fontTable == 0 ) {
		fontTable = &GnuUnicodeFontTable;
	}
	fontManager->setFontTable( fontTable );
	GraphicInfo::setFontManager( fontManager );
	
	//init event manager
	if ( eventManager ) {
		eventManager->setKeyboardListener( keyListener );
		eventManager->setPointingDeviceListener( pointingDeviceListener );
		GraphicInfo::setEventManager( eventManager );
	}

	//
	static sign::sgl::GraphicInfo ginfo( device->deviceName );
	return &ginfo;
}

}
}
