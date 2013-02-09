#include "DrawingTest.h"
#include "TestDevice.h"
#include <gl/glut.h>
#include <cmath>
#include "benchmark.h"
BENCH_INIT();

using namespace sign::sgl;

extern TestDevice *pDevice;
static int wait = 3000;

#define SHOW()	glutPostRedisplay(); \
				::Sleep(wait) \

#define CLEAR()	pDevice->clearScreen()


void
DrawingTest::setUp()
{
}

void
DrawingTest::tearDown()
{
}


void
DrawingTest::test_setPixel()
{
	CLEAR();
	uint32 col = 0xFFFFFFFF;

	for (int y = 0; y < TestDevice::SCREEN_HEIGHT; ++y ) {
		for (int x = 0; x < TestDevice::SCREEN_WIDTH; ++x ) {
			pDevice->setPixel( x, y, col );
			CPPUNIT_ASSERT( pDevice->getPixel(x,y) == col );
		}
		glutPostRedisplay();
		//::Sleep(1);
	}
	SHOW();
}

void
DrawingTest::test_drawLine1()
{
	

	uint32 col = 0xFFFFFFFF;
/*
	(0,0) =>  (2,0)
	     ooo
	     ---
	     ---
*/
	CLEAR();
	pDevice->drawLine( 0, 0, 2, 0, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(3,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(3,1) == 0 );

/*
	(0,0) => (0,2)
	     o--
	     o--
	     o--
*/
	CLEAR();
	pDevice->drawLine( 0, 0, 0, 2, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,2) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,2) == 0 );

/*
	(0,0) => (2,3)
	     o--
	     -o-
	     -o-
	     --o
*/
	CLEAR();
	pDevice->drawLine( 0, 0, 2, 3, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,2) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,3) == col );

/*
	(2,0) => (0,3)
	     --o
	     -o-
	     -o-
	     o--
*/
	CLEAR();
	pDevice->drawLine( 2, 0, 0, 3, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,2) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,3) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,3) == 0 );


/*
	(2,3) =>  (0,0)
	     o--
	     -o-
	     -o-
	     --o
*/
	CLEAR();
	pDevice->drawLine( 2, 3, 0, 0, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,2) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,3) == col );

/*
	(0,3) => (2,0)
	     --o
	     -o-
	     -o-
	     o--
*/
	CLEAR();
	pDevice->drawLine( 0, 3, 2, 0, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,2) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(2,2) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,3) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,3) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(2,3) == 0 );
}

void
DrawingTest::test_drawLine2()
{
	int cx = TestDevice::SCREEN_WIDTH/2;
	int cy = TestDevice::SCREEN_HEIGHT/2;

#define NUM_LINES 64
	int x[NUM_LINES];
	int y[NUM_LINES];
	memset( x, 0, sizeof(x) );
	memset( y, 0, sizeof(y) );

	const double PI_2 = 3.1415926535 * 2;
	const int radius = cy-1;

	for ( int a = 0; a < 360; ++a ) {
		CLEAR();
		double rad = PI_2 * a / 360.0;
		
		for ( int i = NUM_LINES-1; i > 0; --i ) {
			x[i] = x[i-1];
			y[i] = y[i-1];
		}
		x[0] = cos( rad ) * radius;
		y[0] = sin( rad ) * radius;
		
		for ( int i = 0; i < NUM_LINES; ++i ) {
			pDevice->drawLine( cx, cy, cx+x[i], cy+y[i], 0xFFFFFFFF, 1.0-((double)i/NUM_LINES) );
		}
		glutPostRedisplay();
		//::Sleep(50);
	}

	

}

void
DrawingTest::test_fillRectangle()
{
	
	uint32 col = 0xFFFFFFFF;
	CLEAR();
	pDevice->fillRectangle( 0, 0, 0, 0, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );

	CLEAR();
	pDevice->fillRectangle( 0, 0, 1, 0, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );

	CLEAR();
	pDevice->fillRectangle( 0, 0, 1, 1, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );

	CLEAR();
	pDevice->fillRectangle( 0, 0, 2, 1, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );

	CLEAR();
	pDevice->fillRectangle( 0, 0, 2, 2, col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == col );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == col );

	CLEAR();
	pDevice->fillRectangle( 0, 0, 1, 1, col, 0.5 );
	uint32 col_half = 0x007F7F7F;
	CPPUNIT_ASSERT( pDevice->getPixel(0,0) == col_half );
	CPPUNIT_ASSERT( pDevice->getPixel(1,0) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(0,1) == 0 );
	CPPUNIT_ASSERT( pDevice->getPixel(1,1) == 0 );
}


void
DrawingTest::test_putPattern8bpp()
{
	uint8 buf[ 800 * 600 ];

	BENCH_START();
	pDevice->putPattern8bpp( 0, 0, 800, 600, 0, 0, 800, 600, buf, 0x12345678, 1, 0x87654321, 0 );
	BENCH_END( "putPattern8bpp 1" );

	BENCH_START();
	pDevice->putPattern8bpp( 0, 0, 800, 600, 0, 0, 800, 600, buf, 0x12345678, 1, 0x87654321, 0 );
	BENCH_END( "putPattern8bpp 2" );

	BENCH_START();
	pDevice->putPattern8bpp( 0, 0, 800, 600, 0, 0, 800, 600, buf, 0x12345678, 1, 0x87654321, 1 );
	BENCH_END( "putPattern8bpp 3" );

	BENCH_START();
	pDevice->putPattern8bpp( 0, 0, 800, 600, 0, 0, 800, 600, buf, 0x12345678, 1, 0x87654321, 1 );
	BENCH_END( "putPattern8bpp 4" );

}


#include "cover.c"
#include "porsche-240x160.c"
extern bool
decode_main( const unsigned char *file_stream, unsigned char *buf, unsigned int *paletteMem, unsigned int *width, unsigned int *height );

void
DrawingTest::test_showGIF()
{
	const int ROW=160;
	const int COL=240;
	unsigned char imageBuff[ROW*COL];
	unsigned int paletteMem[256];


	CLEAR();
	unsigned int w, h;

	decode_main( cover, imageBuff, paletteMem, &w, &h );
	pDevice->putPattern8bppWithPalette( 0, 0, w, h, (const uint8 *)imageBuff, paletteMem, 0xCCCCCCCC );
	SHOW();

	CLEAR();
	decode_main( porsche, imageBuff, paletteMem, &w, &h );
	pDevice->putPattern8bppWithPalette( 0, 0, w, h, (const uint8 *)imageBuff, paletteMem, 0xCCCCCCCC );
	SHOW();

}


void 
DrawingTest::test_overrun()
{

	uint32 col = 0xFFFFFFFF;
	CLEAR();
	pDevice->drawLine( 0, 0, 799, 599, col );
	pDevice->drawLine( 0, 0, 800, 600, col );
	pDevice->drawLine( 0, 0, 801, 601, col );

	pDevice->drawLine( -1, -1, 799, 599, col );
	pDevice->drawLine( -1, -1, 800, 600, col );
	pDevice->drawLine( -1, -1, 801, 601, col );
	SHOW();

	CLEAR();
	pDevice->fillRectangle( 0, 0, 800, 600, col );
	pDevice->fillRectangle( 0, 0, 801, 601, col );
	
	pDevice->fillRectangle( -1, -1, 800, 600, col );
	pDevice->fillRectangle( -1, -1, 801, 601, col );
	SHOW();
}
