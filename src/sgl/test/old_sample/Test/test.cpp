#include <iostream>
#include <cstdlib> // for rand
#include <ctime>
#include <wchar.h>
#include "sign/sgl++/OpenGLUtils.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/OpenGLDevice.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "sign/sgl++/fonts/gnu_unicode.h"
#include "sign/sgl++/fonts/gnu_unicode_smallCJK.h"

#include "bmpData.inc"
#include "paletteTable.inc"
#include "button2.inc"
#include "button2palette.inc"


using namespace sign::sgl;

static GraphicInfo *ginfo;
#define OPEN_GL_DEVICE_NAME "openGL"
//#define TEST_X 128
//#define TEST_Y 128
#define TEST_X 240
#define TEST_Y 320

inline uint32 
rgb2color( uint8 r, uint8 g, uint8 b)
{
	return OpenGLDevice::device_pixel_type::pack( r, g, b );
}

OpenGLDevice *ogd;
DefaultGraphicDeviceManager *manager;
DefaultFontManager *fontManager;

void systemInit(void)
{
	int argc = 1;
	char *argv[] = { "test", NULL };

	manager = new DefaultGraphicDeviceManager();
	ogd = new OpenGLDevice(OPEN_GL_DEVICE_NAME, TEST_X, TEST_Y, argc, argv);
	manager->setDevice(ogd);
	GraphicInfo::setGraphicDeviceManager(manager);

	fontManager = new DefaultFontManager();
	//fontManager->setFontTable(&hanamiFontTable);
	//fontManager->setFontTable(&GnuUnicodeFontTable);
	fontManager->setFontTable(&gnuUnicodeSmallCJKFontTable);

	GraphicInfo::setFontManager(fontManager);

	GraphicInfo::setEventManager(&OpenGLDevice::theOpenGLEventManager);

	ginfo = new GraphicInfo(OPEN_GL_DEVICE_NAME);
}

static int r[4] = { 2, 4, 8, 16 };
static int old_y[4];

static uint32 savebuf[4][30*30];
//static uint16 savebuf[4][30*30];

void idle(void)
{
	static int count;
	int ocount;

	ginfo->msleep(10);


	static int i[4] = { -30, -30, -30, -30 };
	int j;
	int y, dh;

	for( j = 0 ; j < 4 ; ++j ) {
		if ( i[j] < 0 ) {
			y = 0;
			dh = -i[j];
		} else {
			y = i[j];
			dh = i[j] + 30 - 128;
			if ( dh < 0 ) {
				dh = 0;
			}
		}
		//ginfo->fillRectangle(j * 30, y, 30, 30 - dh, 0x0000);
		if ( count > 0 ) {
			ginfo->putPattern(j * 30, y, 30, 30 - dh, savebuf[j]);
		}
	}
	ocount = count;
	++count;
	for( j = 0 ; j < 4 ; ++j ) {
		if (( count % r[j] ) == 1 ) {
			++i[j];
		}
		if ( i[j] == 128 ) {
			int rr;
			i[j] = -30;
			rr = rand() % 33;
			if ( rr & 1 ) {
				r[j] = 2;
			} else if ( rr & 2 ) {
				r[j] = 4;
			} else if ( rr & 4 ) {
				r[j] = 8;
			} else if ( rr & 8 ) {
				r[j] = 16;
			} else {
				r[j] = (rand() % 19) + 2;
			}
		}
	}
	for( j = 0 ; j < 4 ; ++j ) {
		if ( i[j] < 0 ) {
			y = 0;
			dh = -i[j];
		} else {
			y = i[j];
			dh = i[j] + 30 - 128;
			if ( dh < 0 ) {
				dh = 0;
			}
		}
		ginfo->getPattern(j * 30, y, 30, 30 - dh, savebuf[j]);
		ginfo->fillRectangle(j * 30, y, 30, 30 - dh, 0xFFFF);
	}

redraw:
	glutPostRedisplay();
}

class myKeyboardListener : public KeyboardListener {
public:
	void keyPressed(char key, int32 status) {
		int i = 0;
		switch( key ) {
		case 'h':
		case 'H':
			i = 0;
			break;
		case 'j':
		case 'J':
			i = 1;
			break;
		case 'k':
		case 'K':
			i = 2;
			break;
		case 'l':
		case 'L':
			i = 3;
			break;
		case 'x':
			uint16 buf[100];
			ginfo->getPattern(35, 0, 10, 10, buf);
			for( int x = 0 ; x <= 50; x += 10) {
				for( int y = 0 ; y <= 50; y += 10 ) {
					ginfo->putPattern(x, y, 10, 10, buf);
					//ginfo->repaint();
				}
			}
			//ginfo->msleep(1000);
			break;
		}
		if ( status & SHIFT ) {
			--r[i];
		} else {
			++r[i];
		}
		if ( r[i] < 2 ) {
			r[i] = 2;
		} else if ( r[i] > 16 ) {
			r[i] = 16;
		}
		if ( key == 27 ) { // 27 is esc
			exit(0);
		}
	}
};

class myMouseListener : public PointingDeviceListener {
public:
	void changed(uint32 button, uint32 status, int x, int y) {
		std::cout << "button:" << button << "   "
			<< "status:" << status << "   "
			<< "(" << x << "," << y << ")" << std::endl;
	}
};

uint8 font6x8[]={
	0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00,	// 0	 1
};

int
main(int argc, char *argv[])
{
	systemInit();
	myKeyboardListener *kl;
	myMouseListener *ml;

	kl = new myKeyboardListener();
	ginfo->setKeyboardListener(kl);
	ml = new myMouseListener();
	ginfo->setPointingDeviceListener(ml);

#if 1
	ginfo->fillRectangle(0, 0, 127, 127, 0x0000);
	ginfo->fillRectangle(0, 50, 120, 20, 0x001F);
	ginfo->fillRectangle(0, 0, 40, 40, 0xF800);
	ginfo->fillRectangle(40, 40, 40, 40, 0x07E0);
	ginfo->fillRectangle(80, 80, 40, 40, 0x001F);
	ginfo->fillRectangle(40, 80, 40, 40, 0x0FFF);
	ginfo->putBitPattern(0, 0, 6, 8, font6x8, 0x8000FFFF, 0x000007E0);
	ginfo->putBitPattern(6, 0, 6, 8, font6x8, 0x8000FFFF, 0xFF000000);
	for(int i = 0; i < 120; i += 6) {
		ginfo->putBitPattern(i,  8, 6, 8, font6x8, 0xFFFF, 0x0000);
		ginfo->putBitPattern(i, 16, 6, 8, font6x8, 0x0FF0, 0x0000);
		ginfo->putBitPattern(i, 24, 6, 8, font6x8, 0x0FFF, i/120.0, 0x0000, 1.0 - i/120.0);
		ginfo->putBitPattern(i, 32, 6, 8, font6x8, 0x0FF0, i/120.0, 0x3003, 1.0 - i/120.0);
	}
	ginfo->fillRectangle(60, 60, 40, 40, 0xF100, 0.5);
	ginfo->drawString(30, 100, "abcdefg", 0xFF00, 0x00FF);
	ginfo->drawString(30, 140, L"審美眼", 0xFF00, 0x00FF);
	wchar_t codes[10];
	int i = 0;
	codes[i++] = 0x4f60;
	codes[i++] = 0x597d;
	codes[i++] = 0x0020;
	codes[i++] = 0x313A;
	codes[i++] = 0x3156;
	codes[i++] = 0x0020;
	codes[i++] = 0x2701;
	codes[i++] = 0x3020;
	codes[i++] = 0x0020;
	codes[i++] = 0x2764;
	codes[i++] = 0x0;
	ginfo->drawString(30, 180, codes, 0xFF00, 0x00FF);
	gnuUnicodeSmallCJKFontTable.setFontSize(32, 32);
	ginfo->drawString(10, 80 - 4, "ABCDEF0123XYZ", 0x0FF0, 0.5, 0xF00F, 0.5);

	uint32 buf[20*20];
//	uint16 buf[20*20];
	ginfo->getPattern(100, 10, 20, 20, buf);
	ginfo->putPattern(40, 90, 20, 20, buf);
	for( int i = 0; i <= 100; i += 20 ) {
		ginfo->putPattern(i, 50, 20, 20, buf, ( 20.0 + i ) / 120.0);
	}
	
	glutIdleFunc(idle);
#endif

#if 0
	// clear screen
	ginfo->fillRectangle( 0, 0, TEST_X, TEST_Y,  rgb2color( 0, 0, 0 ) );
	for ( int i = 0; i < 1000; ++i ) {
		int x1 = rand()%TEST_X;	int y1 = rand()%TEST_Y;
		int x2 = rand()%TEST_X;	int y2 = rand()%TEST_Y;
		ginfo->drawLine( x1, y1, x2, y2, rgb2color( 0, 0x80, 0x80 ), 0.5 );
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
		ogd->putBitPattern(130, 110, 8, 32, bp, 0x00FF, 1.0, 0xFF00, 1.0);
	}
#endif


#if 0
	ginfo->drawString( 30, 60, 30, 30, "FAT", rgb2color(0, 0, 255), 1.0, rgb2color(0, 0, 0), 1.0 );
	ginfo->drawString( 31, 60, 30, 30, "FAT", rgb2color(0, 0, 255), 1.0, rgb2color(0, 0, 0), 0.0 );
	ginfo->drawString( 32, 60, 30, 30, "FAT", rgb2color(0, 0, 255), 1.0, rgb2color(0, 0, 0), 0.0 );

	//ginfo->drawString( 30, 120, 24, 24, "日本語表示", 0x7676, 0x0000 );
	//ginfo->drawString( 30, 150, 32, 32, "日本語表示", 0x9595, 0x0000 );

	ginfo->drawString( 32, 102, 24, 24, "Ｓｈａｄｏｗ", rgb2color(0, 0, 0), 1.0, rgb2color(0xFF, 0xFF, 0xFF), 0.8 );
	ginfo->drawString( 30, 100, 24, 24, "Ｓｈａｄｏｗ", rgb2color(0, 0xFF, 0xFF), 1.0, rgb2color(0xFF, 0xFF, 0xFF), 0.0 );
	
	ginfo->drawString( 0, 180, 16, 16, "アカサタナｱｶｻﾀﾅ", rgb2color(0xFF, 0, 0xFF), rgb2color(0xFF, 0xFF, 0xFF) );
	ginfo->drawString( 0, 200, 24, 24, "Alpha Blending Test", rgb2color(0, 0, 0xFF), 1.0, rgb2color(0xFF, 0xFF, 0xFF), 0.0 );
	ginfo->drawString( 0, 240, 24, 24, "Alpha Blending Test", rgb2color(0, 0, 0xFF), 0.5, rgb2color(0xFF, 0xFF, 0xFF), 0.3 );
	ginfo->drawString( 0, 280, 24, 24, "Alpha Blending Test", rgb2color(0, 0, 0xFF), 0.0, rgb2color(0xFF, 0xFF, 0xFF), 1.0 );

	ginfo->drawString( 30, 30, "0123456789", rgb2color( 0xFF, 0, 0 ), rgb2color(0,0,0) );

	fontManager->setFontTable(&hanamiFontTable);
	ginfo->setFont( "hanami_6x8" );
	
	ginfo->drawString( 30, 50, 24,24, "ABCDEFG", rgb2color( 0xFF, 0xFF, 0xFF ), 1.0, rgb2color(0,0,0), 0.0 );
	//ginfo->drawString( 30, 50, "ABCDEFG", rgb2color( 0xFF, 0xFF, 0xFF ), rgb2color(0,0,0) );
#endif


#if 0 //絵文字テスト

	char wc[3];
	wc[0] = 0xF0;
	wc[1] = 0x40;
	wc[2] = '\0';
	ginfo->drawString( 0, 60, 20, 20, wc, rgb2color(0, 0xFF, 0), 1.0, rgb2color(0, 0, 0), 0.0 );

	wc[0] = 0xF8;
	wc[1] = 0xA1;
	wc[2] = '\0';
	ginfo->drawString( 40, 60, 20, 20, wc, rgb2color(0, 0xFF, 0), 1.0, rgb2color(0, 0, 0), 1.0 );

#endif

#if 0 //ビットマップ表示テスト

	ginfo->putPattern8bppWithPalette( 0, 294, 80, 25, bmpData, (uint32*)paletteTable, rgb2color(10, 0, 0) );
	ginfo->putPattern8bppWithPalette( 80, 294, 80, 25, bmpData, (uint32*)paletteTable, rgb2color(10, 0, 0) );
	ginfo->putPattern8bppWithPalette( 160, 294, 80, 25, bmpData, (uint32*)paletteTable, rgb2color(10, 0, 0) );
	ginfo->drawString( 15, 298, 18, 18, "MENU1", rgb2color(0, 0, 0), 1.0, rgb2color(0, 0, 0), 0.0 );
	ginfo->drawString( 95, 298, 18, 18, "MENU2", rgb2color(0, 0, 0), 1.0, rgb2color(0, 0, 0), 0.0 );
	ginfo->drawString( 175, 298, 18, 18, "MENU3", rgb2color(0, 0, 0), 1.0, rgb2color(0, 0, 0), 0.0 );

	ginfo->putPattern8bppWithPalette( 80, 120, 0, 0, 80, 50, 80, 25, button2Data, (uint32*)button2Palette, rgb2color(10, 0, 0) );

	ginfo->putPattern8bppWithPalette( 80, 150, 0, 25, 80, 50, 80, 25, button2Data, (uint32*)button2Palette, rgb2color(10, 0, 0) );


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

	ginfo->doEventLoop();
	
	return 0;
}
