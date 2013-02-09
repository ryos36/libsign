#include "DrawStringTest.h"
#include "TestDevice.h"
#include <gl/glut.h>
#include <cmath>
#include "sign/JPString.h"

using namespace sign;
using namespace sign::sgl;

extern GraphicInfo *pGraphic;
extern TestDevice *pDevice;
static int wait = 250;

#define SHOW()	glutPostRedisplay(); \
				::Sleep(wait) \

#define CLEAR()	pDevice->clearScreen()


void
DrawStringTest::setUp()
{
}

void
DrawStringTest::tearDown()
{
}


void
DrawStringTest::test_drawString1()
{
	CLEAR();

	Rect cliprect( 20, 237, 3, 23 );
	int x = 20;
	JPString str = "ÉuÉbÉNÉ}Å[ÉNÉtÉHÉãÉ_Å[";
	for ( int i = 0 ;i < 10; ++i ) {
		char ch[3];
		JPChar jch = str[i];
		if ( JPString::is_single( jch >> 8 ) ) {
			ch[0] = jch; ch[1] = 0;
		} else {
			ch[0] = jch>>8; ch[1] = jch&0xFF; ch[2] = 0;
		}

		pGraphic->drawString( x, 3, 22, 22, ch, 0xFFFFFFFF, 1, 0, 0, cliprect );
		x += 22;
		SHOW();
	}
	

	pGraphic->drawString( 10, 10, "abcdef", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 10, 30, "abcdef", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 10, 50, "abcdef", 0x00FF0000, 0x00000000 );
	SHOW();

	pGraphic->drawString( 10, 10, "abcdef", 0x000000FF, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 10, 30, "abcdef", 0x0000FF00, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 10, 50, "abcdef", 0x00FF0000, 0x00FFFFFF );
	SHOW();
}

void
DrawStringTest::test_drawString2()
{
	CLEAR();
	pGraphic->drawString( 0, 10, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 0, 30, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 0, 50, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x00FF0000, 0x00000000 );
	SHOW();

	pGraphic->drawString( 0, 10, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x000000FF, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 0, 30, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x0000FF00, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 0, 50, "Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x00FF0000, 0x00FFFFFF );
	SHOW();
}

void
DrawStringTest::test_drawString_size1()
{
	CLEAR();
	pGraphic->drawString( 30, 10, 16, 16, "SMALL", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 30, 30, 32, 32, "MEDIUM", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 30, 70, 48, 48, "LARGE", 0x00FF0000, 0x00000000 );
	SHOW();
}

void
DrawStringTest::test_drawString_size2()
{
	CLEAR();
	pGraphic->drawString( 30, 10, 16, 16, "è¨", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 30, 30, 32, 32, "íÜ", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 30, 60, 48, 48, "ëÂ", 0x00FF0000, 0x00000000 );
	SHOW();
}


void
DrawStringTest::test_drawString_alpha1()
{
	CLEAR();
	pGraphic->fillRectangle( 0, 0, TestDevice::SCREEN_WIDTH, TestDevice::SCREEN_HEIGHT, 0x7F7F7F );
	pGraphic->drawString( 0, 20, 12, 12, "Alpha Blend", 0xFF0000, 1.0, 0xFFFFFF, 0.0 );
	SHOW();
	pGraphic->drawString( 0, 40, 12, 12, "Alpha Blend", 0xFF0000, 0.5, 0xFFFFFF, 0.3 );
	SHOW();
	pGraphic->drawString( 0, 80, 12, 12, "Alpha Blend", 0xFF0000, 0.0, 0xFFFFFF, 1.0 );
	SHOW();
}


void
DrawStringTest::test_drawString_alpha2()
{
}

void
DrawStringTest::test_drawString_clip1()
{
	int x = 30;
	int width = 16;
	for ( int xi = x-16; xi < x+16*3; xi+=4 ) {
		CLEAR();
		pGraphic->drawString( x, 30, 16, 16, "Ç†Ç¢Ç§", 0x000000FF, 0x00000000, Rect( xi, xi + width, 30, 60 ) );
		SHOW();
	}

	int y = 30;
	int height = 16;
	for ( int yi = y-16; yi < y+16; yi+=4 ) {
		CLEAR();
		pGraphic->drawString( 30, y, 16, 16, "Ç†Ç¢Ç§", 0x000000FF, 0x00FFFFFF, Rect( 30, 30 + 16*3, yi, yi+height ) );
		SHOW();
	}
}

void
DrawStringTest::test_drawString_clip2()
{
	unsigned char wc[5];
	wc[0] = 0xF0;
	wc[1] = 0x40;
	wc[2] = 0xF0;
	wc[3] = 0x41;
	wc[4] = 0;

	int x = 30;
	int size = 32;

	for ( int xi = x-16; xi < x+16*2; xi+=4 ) {
		CLEAR();
		pGraphic->drawString( x, 30, 16, 16, (const char*)wc, 0x000000FF, 0x00000000, Rect( xi, xi + size, 30, 60 ) );
		SHOW();
	}
}


void
DrawStringTest::test_drawString_wchar1()
{
	CLEAR();
	pGraphic->drawString( 10, 10, L"abcdef", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 10, 30, L"abcdef", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 10, 50, L"abcdef", 0x00FF0000, 0x00000000 );
	SHOW();

	pGraphic->drawString( 10, 10, L"abcdef", 0x000000FF, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 10, 30, L"abcdef", 0x0000FF00, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 10, 50, L"abcdef", 0x00FF0000, 0x00FFFFFF );
	SHOW();
}


void
DrawStringTest::test_drawString_wchar2()
{
	CLEAR();
	pGraphic->drawString( 0, 10, L"ÉAÉCÉEÉGÉI±≤≥¥µ", 0x000000FF, 0x00000000 );
	SHOW();
	pGraphic->drawString( 0, 30, L"Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x0000FF00, 0x00000000 );
	SHOW();
	pGraphic->drawString( 0, 50, L"Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x00FF0000, 0x00000000 );
	SHOW();

	CLEAR();
	pGraphic->drawString( 0, 10, L"Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x000000FF, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 0, 30, L"Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x0000FF00, 0x00FFFFFF );
	SHOW();
	pGraphic->drawString( 0, 50, L"Ç†Ç¢Ç§ÉAÉCÉE±≤≥", 0x00FF0000, 0x00FFFFFF );
	SHOW();
}


void
DrawStringTest::test_drawString_wchar_clip1()
{
	int x = 30;
	int width = 16;
	for ( int xi = x-16; xi < x+16*3; xi+=4 ) {
		CLEAR();
		pGraphic->drawString( x, 30, 16, 16, L"Ç†Ç¢Ç§", 0x000000FF, 0x00000000, Rect( xi, xi + width, 30, 60 ) );
		SHOW();
	}

	int y = 30;
	int height = 16;
	for ( int yi = y-16; yi < y+16; yi+=4 ) {
		CLEAR();
		pGraphic->drawString( 30, y, 16, 16, L"Ç†Ç¢Ç§", 0x000000FF, 0x00FFFFFF, Rect( 30, 30 + 16*3, yi, yi+height ) );
		SHOW();
	}
}


void
DrawStringTest::test_drawString_wchar_clip2()
{
}


void
DrawStringTest::test_measureString()
{
	int width;
	width = pGraphic->measureString( "ABC", 16 );
	CPPUNIT_ASSERT( width == (16*3)/2 );

	width = pGraphic->measureString( "Ç†Ç¢Ç§", 16 );
	CPPUNIT_ASSERT( width == 16*3 );

	width = pGraphic->measureString( "ÉAÉCÉE", 16 );
	CPPUNIT_ASSERT( width == 16*3 );

	width = pGraphic->measureString( "±≤≥", 16 );
	CPPUNIT_ASSERT( width == (16*3)/2 );

	width = pGraphic->measureString( "ÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇO", 16 );
	CPPUNIT_ASSERT( width == 16*40 );
}



void
DrawStringTest::test_measureString_wchar()
{
	int width;
	width = pGraphic->measureString( L"ABC", 16 );
	CPPUNIT_ASSERT( width == (16*3)/2 );

	width = pGraphic->measureString( L"Ç†Ç¢Ç§", 16 );
	CPPUNIT_ASSERT( width == 16*3 );


	width = pGraphic->measureString( L"ÉAÉCÉE", 16 );
	CPPUNIT_ASSERT( width == 16*3 );

	width = pGraphic->measureString( L"±≤≥", 16 );
	CPPUNIT_ASSERT( width == (16*3)/2 );

	width = pGraphic->measureString( L"ÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÇO", 16 );
	CPPUNIT_ASSERT( width == 16*40 );

}
