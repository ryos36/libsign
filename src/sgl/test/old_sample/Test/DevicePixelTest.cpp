#include "DevicePixelTest.h"
#include "TestDevice.h"
#include "sign/sgl++/DevicePixel.h"
//#include "sign/bench.h"
using namespace sign::sgl;

extern TestDevice *pDevice;

void
DevicePixelTest::setUp()
{
}

void
DevicePixelTest::tearDown()
{
}

void 
DevicePixelTest::test_alphaBlend()
{
//BENCH_INIT();

	unsigned int c = 0xFFFFFFFF;
	unsigned int r0 = ( c & 0x00FF0000 ) >> 16;
	unsigned int r1 = ( c << 8 ) >> 24;

	DevicePixel_A8R8G8B8::pixel_type v;

	v = alphaBlend<DevicePixel_A8R8G8B8>( 0x80, 0, 0, 0xFF, 0, 0x80, 0x80, 0 );
	CPPUNIT_ASSERT( v == 0x00800000 );
	v = alphaBlend<DevicePixel_A8R8G8B8>( 0x80, 0, 0, 0,    0, 0x80, 0x80, 0xFF );
	CPPUNIT_ASSERT( v == 0x00006464 );
	v = alphaBlend<DevicePixel_A8R8G8B8>( 0x80, 0, 0, 0x80, 0, 0x80, 0x80, 0x80 );
	CPPUNIT_ASSERT( v == 0x00404040 );
/*
BENCH_START();
	for ( int i = 0; i < 10000000; ++i ) {
		v = alphaBlend<DevicePixel_A8R8G8B8>( 100, 0, 0, i%256, 0, 100, 100, 128 );
	}
BENCH_END( " test1 " );

BENCH_START();
	for ( int i = 0; i < 10000000; ++i ) {
		v = alphaBlend<DevicePixel_A8R8G8B8>( 100, 0, 0, i%256, 0, 100, 100, 128 );
	}
BENCH_END( " test2 " );

BENCH_START();
	for ( int i = 0; i < 10000000; ++i ) {
		v = alphaBlend<DevicePixel_A8R8G8B8>( 100, 0, 0, i%256, 0, 100, 100, 128 );
	}
BENCH_END( " test3 " );
*/
}

void
DevicePixelTest::test_R5G5B5()
{
	::Sleep(1000);
}

void
DevicePixelTest::test_R5G6B5()
{
}

void
DevicePixelTest::test_R8G8B8A8()
{
}

void
DevicePixelTest::test_A8B8G8R8()
{
}
