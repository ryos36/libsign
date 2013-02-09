
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/OpenGLDevice.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "blue.inc"
#include "water.inc"
#include "jpeg2bmp.h"

using namespace sign::sgl;

static GraphicInfo *ginfo;
#define OPEN_GL_DEVICE_NAME "openGL"
#define TEST_X 400
#define TEST_Y 300

OpenGLDevice *ogd;
DefaultGraphicDeviceManager *manager;
DefaultFontManager *fontManager;


void systemInit(void)
{
	int argc = 1;
	char *argv[] = { "test", NULL };

	manager = new DefaultGraphicDeviceManager();
	ogd = new OpenGLDevice(OPEN_GL_DEVICE_NAME, TEST_X, TEST_Y, argc, argv );
	manager->setDevice(ogd);
	GraphicInfo::setGraphicDeviceManager(manager);
	
	fontManager = new DefaultFontManager();
	fontManager->setFontTable(&hanamiFontTable);
	GraphicInfo::setFontManager(fontManager);

	GraphicInfo::setEventManager(&OpenGLDevice::theOpenGLEventManager);

	ginfo = new GraphicInfo(OPEN_GL_DEVICE_NAME);
}


int main()
{
//#define RESIZE_FUNC		resizeByNearestneighbor
#define RESIZE_FUNC		resizeByBilinear

	systemInit();
	// jpegデータ
	static int	bmpData[ INPUT_SIZE_MAX ];
	static int	dispData[ TEST_X*TEST_Y ];

	RASTER_INFO bmp;
	RASTER_INFO disp;
	
	// jpeg → bmp(?) 変換
	bmp.addr = bmpData;
	jpeg2bmp( &bmp, (const unsigned long*)&blue_data, sizeof(blue_data) );

	// 画面サイズに拡大縮小
	disp.addr = dispData;
	disp.w = 160;
	disp.h = 120;
	RESIZE_FUNC( &disp, &bmp );

	// 変換した bmpを表示
	ginfo->putPattern( 0, 0, 160, 120, disp.addr );

	//-----------------------------------------------------------
	// jpeg → bmp(?) 変換
	bmp.addr = bmpData;
	jpeg2bmp( &bmp, (const unsigned long*)&water_data, sizeof(water_data) );

	// 画面サイズに拡大縮小
	disp.addr = dispData;
	disp.w = 160;
	disp.h = 240;
	RESIZE_FUNC( &disp, &bmp );

	// 変換した bmpを表示
	ginfo->putPattern( 160, 0, 160, 240, disp.addr );


	ginfo->doEventLoop();

	return 0;
}



