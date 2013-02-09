#include <iostream>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestResult.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <process.h>

#include <gl/glut.h>
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/Hanami.h"
#include "sign/sgl++/fonts/gnu_unicode.h"
#include "sign/sgl++/fonts/kelly_lim_font.h"
#include "TestDevice.h"
#include "DevicePixelTest.h"
#include "DrawingTest.h"
#include "FontTest.h"
#include "DrawStringTest.h"


using namespace sign::sgl;


#define NEW_TEST( fixture, test ) new CppUnit::TestCaller<fixture>( "test", &fixture::test )

GraphicInfo *pGraphic;
TestDevice *pDevice;
DefaultFontManager *pFontManager;

//#define DEVICE_PIXEL_TEST
#define DRAWING_TEST
//#define FONT_TEST
//#define DRAW_STRING_TEST

void testMain( void *arg )
{
	{
		CppUnit::TestSuite suite;
		
		//:::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef DEVICE_PIXEL_TEST
		CppUnit::TestSuite *devicePixelTests;
		devicePixelTests = new CppUnit::TestSuite("DevicePixelTest");
		devicePixelTests->addTest( NEW_TEST( DevicePixelTest, test_alphaBlend ) );
		//devicePixelTests->addTest( NEW_TEST( DevicePixelTest, test_R5G5B5 ) );
		//devicePixelTests->addTest( NEW_TEST( DevicePixelTest, test_R5G6B5 ) );
		//devicePixelTests->addTest( NEW_TEST( DevicePixelTest, test_R8G8B8A8 ) );
		//devicePixelTests->addTest( NEW_TEST( DevicePixelTest, test_A8B8G8R8 ) );
		suite.addTest( devicePixelTests );
#endif

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef DRAWING_TEST
		
		CppUnit::TestSuite *drawingTests;
		drawingTests = new CppUnit::TestSuite("DrawingTest");
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_setPixel ) );
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_drawLine1 ) );
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_drawLine2 ) );
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_fillRectangle ) );
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_putPattern8bpp ) );
		drawingTests->addTest( NEW_TEST( DrawingTest, test_showGIF ) );
		//drawingTests->addTest( NEW_TEST( DrawingTest, test_overrun ) );
		
		suite.addTest( drawingTests );
#endif

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef FONT_TEST
		CppUnit::TestSuite *fontTests;
		fontTests = new CppUnit::TestSuite("FontTest");
		fontTests->addTest( NEW_TEST( FontTest, test_render ) );
		
		suite.addTest( fontTests );
#endif

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef DRAW_STRING_TEST
		CppUnit::TestSuite *drawStringTests;
		drawStringTests = new CppUnit::TestSuite("DrawStringTest");
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString2 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_size1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_size2 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_alpha1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_alpha2 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_clip1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_clip2 ) );
		
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_wchar1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_wchar2 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_wchar_clip1 ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_drawString_wchar_clip2 ) );

		//drawStringTests->addTest( NEW_TEST( DrawStringTest, test_measureString ) );
		drawStringTests->addTest( NEW_TEST( DrawStringTest, test_measureString_wchar ) );
		
		suite.addTest( drawStringTests );
#endif		

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::
		CppUnit::TextTestResult result;

		suite.run( &result );
		result.print( std::cout );

	}

	exit(0);
}


int main( int argc, char* argv[] )
{

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(129);
	
	static TestDevice device;
	pDevice = &device;

	static DefaultGraphicDeviceManager manager;
	manager.setDevice( pDevice );
	GraphicInfo::setGraphicDeviceManager( &manager );

	static DefaultFontManager fontManager;
	//fontManager.setFontTable( &hanamiFontTable );
	fontManager.setFontTable( &KellyLimFontTable );
	//fontManager.setFontTable( &GnuUnicodeFontTable );
	pFontManager = &fontManager;
	GraphicInfo::setFontManager( pFontManager );


	static GraphicInfo ginfo("TestDevice");
	pGraphic = &ginfo;

	//----------------------------
	_beginthread( testMain, 0, 0 );

	glutMainLoop();
		
	return 0;
}


