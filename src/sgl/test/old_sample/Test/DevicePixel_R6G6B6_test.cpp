#include "sign/sgl++/DevicePixel.h"
#include <cassert>
#include <cstdio>

using namespace sign::sgl;

int main(int argc, char **argv)
{
	DevicePixel_R6G6B6 dp;
	DevicePixel_R6G6B6::pixel_type pix;
	uint32 r, g, b;
	
	pix = dp.pack( 0, 0, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==0);
	assert(g==0);
	assert(b==0);
	
	pix = dp.pack( 10, 0, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==10);
	assert(g==0);
	assert(b==0);
	
	pix = dp.pack( 10, 10, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==10);
	assert(g==10);
	assert(b==0);
	
	pix = dp.pack( 10, 10, 10 );
        dp.unpack(pix, r, g, b);
	assert(r==10);
	assert(g==10);
	assert(b==10);
				
	pix = dp.pack( 63, 63, 63 );
        dp.unpack(pix, r, g, b);
	assert(r==63);
	assert(g==63);
	assert(b==63);

	//-----------------------------------
	pix = dp.pack( 0, 0, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==0);
	assert(g==0);
	assert(b==0);
	
	pix = dp.pack( 8, 0, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==8);
	assert(g==0);
	assert(b==0);
	
	pix = dp.pack( 8, 8, 0 );
	dp.unpack(pix, r, g, b);
	assert(r==8);
	assert(g==8);
	assert(b==0);

	pix = dp.pack( 248, 248, 248 );
	dp.unpack(pix, r, g, b);
	assert(r==248);
	assert(g==248);
	assert(b==248);

	dp.unpack( 0xFFF0F4F8, r, g, b );
	printf("%x, %x, %x\n", r, g, b);
	assert(r==0xF8);
	assert(g==0xF4);
	assert(b==0xF0);
	
	return 0;
}
