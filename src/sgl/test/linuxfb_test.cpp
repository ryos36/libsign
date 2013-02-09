#include <iostream>
#include <cassert>
#include <cstdlib> // for rand
#include <ctime>
#include <wchar.h>
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "sign/sgl++/fonts/gnu_unicode.h"

#include "LinuxFB.h"
#include "font6x8.cpp"

const unsigned char imgData[] ={
#include "imageData.inc"
};

#include "icon1.inc"
#include "iconPalette.inc"

extern "C" const unsigned char font6x8[];

class MyEventManager : public EventManager {
	virtual int32 getModifiers(void) {
		return 0;
	}

	virtual void doEventLoop(void) {
		while(1);
	}

	virtual void msleep(unsigned int miliseconds) {
		usleep(miliseconds * 1000);
	}
} my_event_man;

#if 0
#include "SglTestSuite.h"
using namespace sign::sgl;
#ifdef SANDGAGE
using namespace sgwp;
#else
using namespace armadillo;
#endif
#endif

GraphicInfo *ginfo;
LinuxFB *sgd;
DefaultGraphicDeviceManager *manager;
DefaultFontManager *fontManager;

inline uint32 
rgb2color( uint8 r, uint8 g, uint8 b)
{
	return LinuxFB::device_pixel_type::pack( r, g, b );
}
#if 0

#ifdef SANDGAGE
SandgateWPLCDDevice *sgd;
#else
Armadillo500FXLCDDevice *sgd;
#endif

// かなりてぬき
class DeviceCreater {
public:
	static GraphicDevice *createDevice() {
		return sgd;
	}
	
	static void deleteDevice() {
	}
};

void deviceTest(GraphicDevice *tmp)
{
	test::DeviceTest<DeviceCreater> ts;
	ts.preTest();
	assert(tmp == ts.device);
	
	ts.test();
	/*
	ts test 
	gd->setPixel(0, 0, 0xFFFFFF);
	gd->setPixel(1, 1, 0xFFFFFF);
	gd->drawLine(0, 0, 239, 319, 0xFFFFFF);
	gd->fillRectangle(0, 0, 240, 320, 0x0000FF);
	*/
}
#endif

void systemInit(void)
{
	sgd = LinuxFB::createLCDDevice("linux-fb");
	//deviceTest(sgd);

	manager = new DefaultGraphicDeviceManager();
	manager->setDevice(sgd);
	GraphicInfo::setGraphicDeviceManager(manager);

	fontManager = new DefaultFontManager();
	fontManager->setFontTable(&hanamiFontTable);
	//fontManager->setFontTable(&GnuUnicodeFontTable);
	//fontManager->setFontTable(&KellyLimFontTable);
	//fontManager->setFontTable(&SglDefault16FontTable);

	GraphicInfo::setFontManager(fontManager);

	//GraphicInfo::setEventManager(&OpenGLDevice::theOpenGLEventManager);
	GraphicInfo::setEventManager(&my_event_man);

	ginfo = new GraphicInfo("linux-fb");
	std::cout << "ginfo " << (void *)ginfo << std::endl;
}

int
main(int argc, char *argv[])
{
	std::cout << "run" << std::endl;
	systemInit();
	std::cout << "init ok" << std::endl;
#if 1
	ginfo->fillRectangle( 0,  0, 127, 127, 0xFF000000);
	ginfo->fillRectangle( 0, 50, 120,  20, 0xFF00001F);
	ginfo->fillRectangle( 0,  0,  40,  40, 0xFF00F800);
	ginfo->fillRectangle(40, 40,  40,  40, 0xFF0007E0);
	ginfo->fillRectangle(80, 80,  40,  40, 0xFF00001F);
	ginfo->fillRectangle(40, 80,  40,  40, 0xFF000FFF);
#endif

#if 1
	ginfo->putBitPattern(0, 0, 6, 8, font6x8, 0x8000FFFF, 0x000007E0);
	ginfo->putBitPattern(6, 0, 6, 8, font6x8, 0x8000FFFF, 0xFF000000);
	for(int i = 0; i < 120; i += 6) {
		ginfo->putBitPattern(i,  8, 6, 8, font6x8, 0xFFFF, 0xFF000000);
		ginfo->putBitPattern(i, 16, 6, 8, font6x8, 0x0FF0, 0xFF000000);
		ginfo->putBitPattern(i, 24, 6, 8, font6x8, 0x0FFF, i/120.0, 0x0000, 1.0 - i/120.0);
		ginfo->putBitPattern(i, 32, 6, 8, font6x8, 0x0FF0, i/120.0, 0x3003, 1.0 - i/120.0);
	}
	ginfo->fillRectangle(60, 60, 40, 40, 0xFF00F100, 0.5);
	ginfo->drawString(30, 100, "abcdefg", 0xFF00FF00, 0xFF0000FF);
	ginfo->drawString(30, 80 - 4, "ABCDEFG0123XYZ", 0xFF000FF0, 0.5, 0xFF00F00F, 0.5);
	wchar_t codes[10];
	int i = 0;
	codes[i++] = 0x4f60;
	codes[i++] = 0x597d;
	codes[i++] = 0x9648;
	codes[i++] = 0x6c34;
	codes[i++] = 0x6241;
	codes[i++] = 0x6731;
	codes[i++] = 0x5c0f;
	codes[i++] = 0x78ca;
	codes[i++] = 0x0;
	ginfo->drawString(30, 180, codes, 0xFF00FF00, 0xFF0000FF);

	uint32 buf[20*20];
//	uint16 buf[20*20];
	ginfo->getPattern(100, 10, 20, 20, buf);
	ginfo->putPattern(40, 90, 20, 20, buf);
	for( int i = 0; i <= 100; i += 20 ) {
		ginfo->putPattern(i, 50, 20, 20, buf, ( 20.0 + i ) / 120.0);
	}
	
#endif


#if 0
	{
		ginfo->drawString(30, 10, "abcdefg", 0xFF00, 0x00FF);
		unsigned int code32;
		code32 = 0x31;
		code32 = 0xfcc6;
		code32 = 0xf3;
		uint8 *bp;
		bp = (uint8 *)GnuUnicodeFontTable.getPattern(code32);
		sgd->putBitPattern(130, 110, 8, 32, bp, 0x00FF, 1.0, 0xFF00, 1.0);
	}
#endif

#if 0
	//unsigned char *frame = (unsigned char*)sgd->getFrameBuffer();
	
	std::cout << "fillRectangle" << std::endl;
	ginfo->fillRectangle( 0, 0, 240, 320, 0, 1 );

	RGBAColor red(0xFF, 0, 0);
	RGBAColor green(0, 0xFF, 0);
	RGBAColor blue(0, 0, 0xFF);
	RGBAColor kellywhite( 0xF8, 0xF4, 0xF0 );
	//RGBAColor titleBlack( 0x14, 0x10, 0x10, 0xFF);
	ginfo->fillRectangle( 0, 0, 50, 50, red.rgb2PixelValue(), 1);
	//printf("frame = %x %x %x %x \n", frame[0], frame [1], frame [2], frame [3]);
	ginfo->fillRectangle( 0, 50, 50, 50, green.rgb2PixelValue(), 1);
	//printf("frame = %x %x %x %x \n", frame[0], frame [1], frame [2], frame [3]);
	ginfo->fillRectangle( 0, 100, 50, 50, blue.rgb2PixelValue(), 1);
	//printf("frame = %x %x %x %x \n", frame[0], frame [1], frame [2], frame [3]);
	ginfo->fillRectangle( 0, 150, 50, 50, kellywhite.rgb2PixelValue(), 1);
	//printf("frame = %x %x %x %x \n", frame[0], frame [1], frame [2], frame [3]);
	
#endif

#if 1
	//printf("putPattern32bpp\n");
	ginfo->putPattern32bpp( 0, 0, 240, 320, 0, 0, 240, 320, (const uint32*)imgData, 0xFF );
#endif

#if 0
	//printf("putPattern8bppWithPalette\n");
	ginfo->putPattern8bppWithPalette( 0, 0, 240, 320, icon1, (uint32*)iconPalette, rgb2color(10, 0, 0) );


#endif


#if 0
	{
		uint8 *bp;
		uint32 code, wi, hi;
		code = 0x21;
		//GnuUnicodeFontTable;
		//GnuUnicodeFontTable.setFontSize(32, 32);
		wi = SglDefault16FontTable.getCodeWidth(code);
		hi = SglDefault16FontTable.getHeight();
		bp = (uint8 *)SglDefault16FontTable.getPattern(code);
		std::cout << wi << " " << hi << std::endl;
		for ( int h = 0 ; h < hi; h++ ) {
			for ( int w = 0 ; w < wi; w += 8, ++bp ) {
				uint8 mask = 0x80;
				for( int w8 = 0 ; w8 < 8; w8++, mask >>= 1 ) {
					if ( *bp & mask ) {
						std::cout << '*';
					} else {
						std::cout << '_';
					}
				}
			}
			std::cout << std::endl;
		}
	}
#endif


	//vx_scheme_main(argc, argv);
	ginfo->doEventLoop();
	
	return 0;
}
