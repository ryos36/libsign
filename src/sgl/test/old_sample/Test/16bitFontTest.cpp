#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/OpenGLDevice.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/Hanami.h"

#include "sign/sgl++/fonts/kelly_lim_font.h"

#include "paletteTable.inc"

#include <stdio.h>

#include "put_font_mem_16bit.h"

using namespace sign::sgl;

static GraphicInfo *ginfo;
#define OPEN_GL_DEVICE_NAME "16bit Font Test"
#define TEST_X 240
#define TEST_Y 320

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
	systemInit();

	KellyLimFontTable::setPaletteTable( &paletteTable[0] );

#define FONT_SIZE_GAIZI	22
#define FONT_SIZE_EMOZI	20
#define BUF_SIZE_X	64
#define BUF_SIZE_Y	128
	static unsigned short sbuf[BUF_SIZE_X * BUF_SIZE_Y];
	CRect bitMap =	{ 0, 0, BUF_SIZE_X, BUF_SIZE_Y };
	CRect fontEmozi =	{ 0, 0, FONT_SIZE_EMOZI, FONT_SIZE_EMOZI };
	CRect fontGaizi =	{ 0, 0, FONT_SIZE_GAIZI, FONT_SIZE_GAIZI };
	CColor color =	{ 0xFF, 0x00, 0x00 };

	short str_emozi[] 		=  { 0x40f2, 0 };
	short str_emozi_fin[] 	=  { 0x4af5, 0 };
	short str_gaizi[] 		=  { 0x4ef8, 0 };
	short str_gaizi_fin[] 	=  { 0x4ff8, 0 };

	short str_emoji2[] 		=  { 0x40f0, 0 };

//	put_font_mem_16bit( &sbuf[0], &bitMap, 0, ( char * ) &str_gaizi[0], &fontGaizi, &color );

	put_font_mem_16bit( &sbuf[0], &bitMap, 0, ( char * ) &str_emozi[0], &fontEmozi, &color );
/*
	for ( int i = 0; i < BUF_SIZE_Y; i++ ) {
		for ( int j = 0; j < BUF_SIZE_X; j++ ) {
			if ( 0 == sbuf[i * BUF_SIZE_X + j] ) {
				printf( " " );
			} else {
				printf( "*" );
			}
		}
		printf( "\n" );
	}
*/

	put_font_mem_16bit( &sbuf[FONT_SIZE_GAIZI * BUF_SIZE_X], &bitMap, 0, ( char * ) &str_gaizi[0], &fontGaizi, &color );

	put_font_mem_16bit( &sbuf[FONT_SIZE_GAIZI * BUF_SIZE_X * 2], &bitMap, 0, ( char * ) &str_emozi_fin[0], &fontEmozi, &color );
	put_font_mem_16bit( &sbuf[FONT_SIZE_GAIZI * BUF_SIZE_X * 3], &bitMap, 0, ( char * ) &str_gaizi_fin[0], &fontGaizi, &color );
	put_font_mem_16bit( &sbuf[FONT_SIZE_GAIZI * BUF_SIZE_X * 4], &bitMap, 0, ( char * ) &str_emoji2[0], &fontEmozi, &color );
	put_font_mem_16bit( &sbuf[FONT_SIZE_GAIZI * BUF_SIZE_X * 5], &bitMap, 0, ( char * ) "‚ ", &fontEmozi, &color );


#define COLOR_NF16TO32( _color_ ) \
        (( 0x7C00 & _color_ ) >> 7) | \
        (( 0x03E0 & _color_ ) << 6 ) | \
        (( 0x001F & _color_ ) << 19 )

	static unsigned int ibuf[BUF_SIZE_X * BUF_SIZE_Y];
	for ( int i = 0; i < BUF_SIZE_Y; i++ ) {
		for ( int j = 0; j < BUF_SIZE_X; j++ ) {
			uint32 x = sbuf[i * BUF_SIZE_X + j];
			if ( x == 0x7d93 ){
				//ASSERT(0);
			}
			
			uint32 col = COLOR_NF16TO32(x);
			ibuf[i * BUF_SIZE_X + j] = col;
		}
	}
	
	ginfo->putPattern( 0, 0, BUF_SIZE_X, BUF_SIZE_Y, &ibuf[0] );
	ginfo->doEventLoop();

	return 0;
}
