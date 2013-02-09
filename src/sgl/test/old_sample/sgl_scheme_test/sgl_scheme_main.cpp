#include <iostream>

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


using namespace sign::sgl;


#define NEW_TEST( fixture, test ) new CppUnit::TestCaller<fixture>( "test", &fixture::test )

GraphicInfo *pGraphic;
TestDevice *pDevice;
DefaultFontManager *pFontManager;




extern int scheme_main ( int argc, char **argv );
//-------------------------------------------------------------
void consoleMain( void *arg )
{
	char *argv[] = { "" };
	scheme_main( 1, argv );
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

	_beginthread( consoleMain, 0, 0 );

	glutMainLoop();
		
	return 0;
}


