#include <cstdlib>
#include "sgl_init.h"
#include "sign/sgl++/DevicePixel.h"
#include <cmath>
#include <cassert>

//#define SGL_SAMPLE_DEBUG

#ifdef SGL_SAMPLE_DEBUG
#include <iostream>
#endif

unsigned char bmpdata[] = {
#include "sinby_logo.bmp.inc"
};

#if (defined(_MSC_VER) || defined(__CYGWIN__))
	#if defined(__CYGWIN__)
		#include <windows.h>
	#endif
	#include "sign/sgl++/OpenGLUtils.h"
	#include "OpenGLDevice.h"
	#define FB_DEVICE OpenGLDevice
	#define FB_COLOR_DEPTH_R 8
	#define FB_COLOR_DEPTH_G 8
	#define FB_COLOR_DEPTH_B 8
	#include <mmsystem.h>
	#define GET_SYSTEM_TIME() timeGetTime()
	#define MSLEEP(m) Sleep(m)
#else /* for linux */
	#if defined(__ARMADILLO500FX__)
		#include "Armadillo500FXLCDDevice.h"
		#define FB_DEVICE Armadillo500FXLCDDevice
		#define FB_COLOR_DEPTH_R 5
		#define FB_COLOR_DEPTH_G 6
		#define FB_COLOR_DEPTH_B 5
	#else
		#include "VesaVgaDevice.h"
		#define FB_DEVICE VesaVgaDevice
		#define FB_COLOR_DEPTH_R 8
		#define FB_COLOR_DEPTH_G 8
		#define FB_COLOR_DEPTH_B 8
	#endif
	#include <sys/time.h>
	uint32 GET_SYSTEM_TIME()
	{
		timeval tv;
		gettimeofday(&tv, NULL);
		
		uint32 msec = (tv.tv_sec*1000) + (tv.tv_usec/1000) ;

		return msec;
	}
	#include <unistd.h>
	#define MSLEEP(m) usleep(m*1000)
#endif



using namespace sign;
using namespace sign::sgl;

void idle(void);
//-------------------------------------------------------------
void sample1(void);//random fill rect
void sample2(void);//random fill rect alpha
void sample3(void);//gradation
void sample4(void);//alpha gradation
void sample5(void);//sinby logo image
void sample6(void);//whiteout
void sample7(void);//objects
void sample8(void);//texts
void sample9(void);//mozaic
void sample10(void);//bound box
void sample11(void);//blur
void sample12(void);//edge
void sample13(void);//turtle
//-------------------------------------------------------------
void sample21(void);//reverse(up<->down)
void sample22(void);//reverse(left<->right)
void sample23(void);//wipeout
void sample100(void);//debug


void randFillRect(int size, double alpha);
void convolution( FB_DEVICE::pixel_type sbuf[], FB_DEVICE::pixel_type dbuf[], double mtrx[][3] );

typedef void (*SampleFunc)(void) ;
const int MAX_SAMPLE = 16;
SampleFunc sequence[MAX_SAMPLE] = 
{
	
	sample5,
	
	sample1,
	sample3,
	sample13,
	sample8,
	sample21,
	sample22,

	sample2,
	sample11,
	sample7,
	sample12,
	
	sample10,
	sample23,
	
	sample4,
	sample9,
	sample6,
	
};
SampleFunc sampleFn;
static uint32 currentSampleIndex = 0;

sign::sgl::GraphicInfo *ginfo;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
const int MAX_OBJ = 30;
const int BOX_SIZE = 80;
const int MAX_BOX_X_N = SCREEN_WIDTH / BOX_SIZE;
const int MAX_BOX_Y_N = SCREEN_HEIGHT / BOX_SIZE;
const int MAX_BOX = MAX_BOX_X_N * MAX_BOX_Y_N;
const double PI = 3.14159265358979;
const int CENTER_X = SCREEN_WIDTH/2;
const int CENTER_Y = SCREEN_HEIGHT/2;
const int PIXEL_SIZE = sizeof(FB_DEVICE::pixel_type);
const uint8 MAX_COLOR_R = (uint8)pow(2, FB_COLOR_DEPTH_R);
const uint8 MAX_COLOR_G = (uint8)pow(2, FB_COLOR_DEPTH_G);
const uint8 MAX_COLOR_B = (uint8)pow(2, FB_COLOR_DEPTH_B);
const uint8 MAXR = MAX_COLOR_R-1;
const uint8 MAXG = MAX_COLOR_G-1;
const uint8 MAXB = MAX_COLOR_B-1;
const uint32 WHITE = FB_DEVICE::device_pixel_type::pack(MAXR, MAXG, MAXB);
const uint32 BLACK = FB_DEVICE::device_pixel_type::pack(0, 0, 0);

bool start = true;
uint32 oldt;
FB_DEVICE *device;

//-------------------------------------------------------------
uint8 rand_r()
{
	return rand() % MAX_COLOR_R;
}

uint8 rand_g()
{
	return rand() % MAX_COLOR_G;
}

uint8 rand_b()
{
	return rand() % MAX_COLOR_B;
}

uint32 rand_color()
{
	uint8 r = rand_r();
	uint8 g = rand_g();
	uint8 b = rand_b();
	return FB_DEVICE::device_pixel_type::pack(r, g, b);
}

//-------------------------------------------------------------
struct Object
{
	enum {
		N = 10,
		SIZE_X = 3,
		SIZE_Y = 3
	};

	Object(sign::sgl::GraphicInfo *gi)
		: color(0), speed(0), ginfo(gi)
	{
		update();
	}

	void tick(void)
	{
		if (delay > 0) {
			--delay;
			return;
		}

		for (int i = 0; i < N; ++i) {
			ginfo->fillRectangle((int32)x, (int32)y[i], SIZE_X, SIZE_Y, color, alpha[i]);
		}

		for (int i = 0; i < N-1; ++i) {
			alpha[i+1] = alpha[i]*0.7;
		}

		for (int i = N-1; i > 0; --i) {
			y[i] = y[i-1];
		}

		alpha[0] += speed;
		if ( alpha[0] > 1 ) {
			speed = -speed;
			alpha[0] = 1;
		}
		else if ( alpha[0] < 0 ) {
			update();
		}
		
		y[0] += 1;
		if ( y[0] > (SCREEN_HEIGHT - SIZE_Y) ) {
			update();
		}
	}

	void update(void)
	{
		uint8 r = 0;//(rand() % 256);
		uint8 g = 0;//(rand() % 256);
		uint8 b = rand_b();//(rand() % 256);
		color = FB_DEVICE::device_pixel_type::pack(r, g, b);
		alpha[0] = 0;
			
		x = rand() % (SCREEN_WIDTH - SIZE_X);
		y[0] = rand() % (SCREEN_HEIGHT - 16);
		for (int i = 1; i < N; ++i) {
			alpha[i] = alpha[0];
			y[i] = y[0];
		}

		speed = ((double)rand() / RAND_MAX) * 0.05 + 0.005;
		assert( speed > 0 );

		delay = rand() % 50;
	}

	uint32 color;
	double x;
	double y[N];
	double alpha[N];
	double speed;
	int32 delay;

	sign::sgl::GraphicInfo *ginfo;
};

Object *objs[MAX_OBJ];

struct Object2
{
	Object2(const char *t, sign::sgl::GraphicInfo *gi)
		: text(t), color(0), speed(0), ginfo(gi)
	{
		tw = ginfo->measureString(text, 16);
		update();
	}

	void tick(void)
	{
		if (delay > 0) {
			--delay;
			return;
		}

		ginfo->drawString((int32)x, (int32)y, text, color, alpha, 0, 0);
		
		alpha += speed;
		if ( alpha > 1 ) {
			speed = -speed;
			alpha = 1;
		}
		else if ( alpha < 0 ) {
			update();
		}
	}

	void update(void)
	{
		color = rand_color();
		alpha = 0;
			
		x = rand() % (SCREEN_WIDTH - tw);
		y = rand() % (SCREEN_HEIGHT - 16);
		
		speed = ((double)rand() / RAND_MAX) * 0.05 + 0.005;
		assert( speed > 0 );

		delay = rand() % 50;
	}

	const char *text;
	uint32 tw;

	uint32 color;
	double x;
	double y;
	double alpha;
	double speed;
	int32 delay;

	sign::sgl::GraphicInfo *ginfo;
};

Object2 *objs2[MAX_OBJ];

//-------------------------------------------------------------
struct Box
{
	int32 x;
	int32 y;
	int32 ax;
	int32 ay;
	uint32 bitmapx;
	uint32 bitmapy;
	const uint32 *bitmap;
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];

	Box( uint32 x, uint32 y)
		:x(x), y(y), bitmapx(x), bitmapy(y)
	{
	}

	void reset(void)
	{
		ax = rand() % 9 + 1;
		ay = rand() % 9 + 1;
		x = bitmapx;
		y = bitmapy;
	}

	void tick(void)
	{
		x += ax;
		
		if (x < 0) {
			x = 0;
			ax = -ax;
		} else if (x+BOX_SIZE > SCREEN_WIDTH) {
			x = SCREEN_WIDTH-BOX_SIZE;
			ax = -ax;
		}

		y += ay;
		if (y < 0) {
			y = 0;
			ay = -ay;
		} else if (y+BOX_SIZE > SCREEN_HEIGHT) {
			y = SCREEN_HEIGHT-BOX_SIZE;
			ay = -ay;
		}

		ginfo->putPattern(x, y, BOX_SIZE, BOX_SIZE, bitmapx, bitmapy, SCREEN_WIDTH, SCREEN_HEIGHT, buf, 0xCCCCCCCC);
	}
};
FB_DEVICE::pixel_type Box::buf[SCREEN_SIZE];

Box *boxes[MAX_BOX];


//-------------------------------------------------------------
class Turtle
{
public:
	Turtle()
	{
		reset();
	}

	void move( int n )
	{
		x1 = x0 + (double)n * sin( PI * direction / 180.0 );
		if ( x1 < 0 ) x1 = 0;
		if ( x1 >= SCREEN_WIDTH ) x1 = SCREEN_WIDTH-1;
		y1 = y0 - (double)n * cos( PI * direction / 180.0 );
		if ( y1 < 0 ) y1 = 0;
		if ( y1 >= SCREEN_HEIGHT ) y1 = SCREEN_HEIGHT-1;

		ginfo->drawLine( (int)x0, (int)y0, (int)x1, (int)y1, color, 0.25 );
		x0 = x1;
		y0 = y1;
	}

	void turn( double digree )
	{
		direction += digree;
	}

	void reset()
	{
		x0 = CENTER_X;
		y0 = CENTER_Y;
		x1 = CENTER_X;
		y1 = CENTER_Y;
		direction = 0;
		color = WHITE; 
	}

	void setColor(uint32 c) { color = c; }
private:
	double x0, y0, x1, y1;
	double direction;
	uint32 color;
};

Turtle turtle;

//-------------------------------------------------------------
void
shuffleSample()
{
#ifndef SGL_SAMPLE_DEBUG
	SampleFunc tmp;
	for (int i = 0; i < 30; ++i) {
		int a = rand() % MAX_SAMPLE;
		int b = rand() % MAX_SAMPLE;
		tmp = sequence[a];
		sequence[a] = sequence[b];
		sequence[b] = tmp;
	}
#endif
}




//-------------------------------------------------------------
int main( int argc, char* argv[] )
{
#if (defined(_MSC_VER) || defined(__CYGWIN__))
	OpenGLDevice openGLDevice("OpenGL", SCREEN_WIDTH, SCREEN_HEIGHT, argc, argv );
	device = &openGLDevice;
	ginfo = sign::sgl::initGraphicInfo( &openGLDevice, 0, 0, 0, &OpenGLDevice::theOpenGLEventManager, 0, 0 );
#else /* for linux */
	device = FB_DEVICE::createDevice();
	ginfo = sign::sgl::initGraphicInfo( device, 0, 0, 0, 0, 0, 0 );
#endif

#if (defined(_MSC_VER) || defined(__CYGWIN__))
	glutIdleFunc(idle);
#endif
	
	oldt = GET_SYSTEM_TIME();
	srand(oldt);

	currentSampleIndex = 0;
	sampleFn = sequence[currentSampleIndex];

	for (int i =0; i < MAX_OBJ; ++i) {
		objs[i] = new Object(ginfo);
		objs2[i] = new Object2("Sinby Graphic Library", ginfo);
	}

	for (int y =0; y < MAX_BOX_Y_N; ++y) {
		for (int x =0; x < MAX_BOX_X_N; ++x) {
			boxes[y*MAX_BOX_X_N+x] = new Box(x*BOX_SIZE, y*BOX_SIZE);
		}
	}

#if (defined(_MSC_VER) || defined(__CYGWIN__))
	glutMainLoop();
#else
	while(1) {
		idle();
		MSLEEP(10);
	}
#endif
	
	return 0;
}


//-------------------------------------------------------------
void idle(void)
{
	uint32 t = GET_SYSTEM_TIME();
	uint32 elapsed = t - oldt;

	if ( elapsed >= 10000 ) {

#ifdef SGL_SAMPLE_DEBUG
	printf("%d:Index %d end.\n", t, currentSampleIndex);
#endif

		++currentSampleIndex;	
		if ( currentSampleIndex >= (uint32)MAX_SAMPLE ) {
			shuffleSample();
			currentSampleIndex = 0;
		}
		sampleFn = sequence[currentSampleIndex];
		
		start = true;
		oldt = t;
	}

	(*sampleFn)();

	MSLEEP(1);
	
#if (defined(_MSC_VER) || defined(__CYGWIN__))
	glutPostRedisplay();
#endif
}


//-------------------------------------------------------------
//random fill rect
void sample1(void)
{
	static const int32 Size = 40;
	randFillRect(Size, 1.0);
}


//-------------------------------------------------------------
//random fill rect with alpha
void sample2(void)
{
	static const int32 Size = 40;
	randFillRect(Size, 0.5);
}

//-------------------------------------------------------------
void randFillRect(int size, double alpha)
{
	int32 x = rand() % (SCREEN_WIDTH - size);
	int32 y = rand() % (SCREEN_HEIGHT - size);

	uint32 color = rand_color();

	ginfo->fillRectangle(x, y, size, size, color, alpha);
	ginfo->drawLine(x, y, x+size-1, y, 0 );
	ginfo->drawLine(x+size-1, y, x+size-1, y+size-1, 0 );
	ginfo->drawLine(x, y, x, y+size-1, 0 );
	ginfo->drawLine(x, y+size-1, x+size-1, y+size-1, 0 );
	
}

//-------------------------------------------------------------
//gradation
void sample3(void)
{
	static int32 y;
	static uint8 r, g, b;
	if (start) {
		y = 0;
		r = MAXR - rand_r();
		g = MAXG - rand_g();
		b = MAXB - rand_b();
		
		start = false;
	}
	
	for (int i = 0; i < 2; ++i ) {
		double v = (double)y/SCREEN_HEIGHT;
		uint8 r0 = MAXR - (int)(v*r);
		uint8 g0 = MAXG - (int)(v*g);
		uint8 b0 = MAXB - (int)(v*b);
		uint32 color = FB_DEVICE::device_pixel_type::pack(r0, g0, b0);
		ginfo->drawLine(0, y, SCREEN_WIDTH-1, y, color);
		y += 1;
		
		if (y >= SCREEN_HEIGHT) {
			oldt = 0;
			return;
		}
	}
}

//-------------------------------------------------------------
//alpha gradation
void sample4(void)
{
	static int32 y;
	static uint32 color;
	if (start) {
		y = 0;
		color = rand_color();
		start = false;
	}
	
	for (int i = 0; i < 2; ++i ) {
		ginfo->drawLine(0, y, SCREEN_WIDTH-1, y, color, (double)y/SCREEN_HEIGHT );
		y += 1;
		if (y >= SCREEN_HEIGHT) {
			oldt = 0;
			return;
		}
	}
}


//-------------------------------------------------------------
//sinby logo image
void sample5(void)
{
	static int32 y;
	if (start) {
		y = (SCREEN_HEIGHT - 64);
		start = false;
	}

	if (y < 0) {
		oldt = 0;
		return;
	}
	ginfo->putPattern32bpp(0, y, SCREEN_WIDTH, 64, (const uint32*)bmpdata);
	y -= 2;
}


//-------------------------------------------------------------
//whiteout
void sample6(void)
{
	ginfo->fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE, 0.06);
	MSLEEP(100);
}


//-------------------------------------------------------------
//objects
void sample7(void)
{
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		start = false;
	}

	ginfo->putPattern(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, buf);
	for (int i =0; i < MAX_OBJ; ++i) {
		objs[i]->tick();
	}
}

//-------------------------------------------------------------
//texts
void sample8(void)
{
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		for (int i =0; i < MAX_OBJ; ++i) {
			objs2[i]->update();
		}
		start = false;
	}

	ginfo->putPattern(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, buf);
	for (int i =0; i < MAX_OBJ; ++i) {
		objs2[i]->tick();
	}
}

//-------------------------------------------------------------
//mozaic
void sample9(void)
{
	static FB_DEVICE::pixel_type sbuf[SCREEN_SIZE];
	static FB_DEVICE::pixel_type dbuf[SCREEN_SIZE];

	static double dd;
	if (start) {
		memcpy(sbuf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		memcpy(dbuf, sbuf, SCREEN_SIZE * PIXEL_SIZE);
		start = false;
		dd = 1;
	}

	uint32 d = (uint32)dd;
	for(int y = 0; y < SCREEN_HEIGHT; y += d) {
		for(int x = 0; x < SCREEN_WIDTH; x += d) {
			FB_DEVICE::pixel_type pix = sbuf[y*SCREEN_WIDTH + x];
			for(uint32 dy = 0; dy < d; ++dy) {
				for(uint32 dx = 0; dx < d; ++dx) {
					if ( y+dy < (uint32)SCREEN_HEIGHT && x+dx < (uint32)SCREEN_WIDTH ) {
						dbuf[(y+dy)*SCREEN_WIDTH + x+dx] = pix;
					}
				}
			}
		}
	}

	ginfo->putPattern(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, dbuf);
	dd += 0.05;
}

//-------------------------------------------------------------
//bound box
void sample10(void)
{
	if (start) {
		memcpy(Box::buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		for (int i =0; i < MAX_BOX; ++i) {
			boxes[i]->reset();
		}
		start = false;
	}

	for (int i =0; i < MAX_BOX; ++i) {
		boxes[i]->tick();
	}
}

//-------------------------------------------------------------
//blur
void sample11(void)
{
	static double matrix[3][3] = 
	{
		{ 0.1, 0.1, 0.1 }, 
		{ 0.1, 0.2, 0.1 }, 
		{ 0.1, 0.1, 0.1 }
	};


	static FB_DEVICE::pixel_type sbuf[SCREEN_SIZE];
	static FB_DEVICE::pixel_type dbuf[SCREEN_SIZE];
	
	if (start) {
		memcpy(sbuf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		memcpy(dbuf, sbuf, SCREEN_SIZE * PIXEL_SIZE);
		start = false;
	}

	convolution(sbuf, dbuf, matrix);
	ginfo->putPattern(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, dbuf);
	memcpy(sbuf, dbuf, SCREEN_SIZE * PIXEL_SIZE);
}

//-------------------------------------------------------------
//edge
void sample12(void)
{
	static double matrix[3][3] = 
	{
		{  0.0,  -0.2,  0.0 }, 
		{ -0.2,   1.8,  -0.2 }, 
		{  0.0,  -0.2,  0.0 }
	};

	static FB_DEVICE::pixel_type sbuf[SCREEN_SIZE];
	static FB_DEVICE::pixel_type dbuf[SCREEN_SIZE];
	
	if (start) {
		memcpy(sbuf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		memcpy(dbuf, sbuf, SCREEN_SIZE * PIXEL_SIZE);
		start = false;
	}

	convolution(sbuf, dbuf, matrix);
	ginfo->putPattern(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, dbuf);
	memcpy(sbuf, dbuf, SCREEN_SIZE * PIXEL_SIZE);
}

//-------------------------------------------------------------
void
convolution( FB_DEVICE::pixel_type sbuf[], FB_DEVICE::pixel_type dbuf[], double mtrx[][3] )
{
	for(int y = 1; y+1 < SCREEN_HEIGHT; ++y) {
		for(int x = 1; x+1 < SCREEN_WIDTH; ++x) {
			uint8 r=0, g=0, b=0;
			
			for(int dy = 0; dy < 3; ++dy) {
				for(int dx = 0; dx < 3; ++dx) {

					double bl = mtrx[dy][dx];
					FB_DEVICE::pixel_type pix0 = sbuf[(y+dy-1)*SCREEN_WIDTH + x+dx-1];
					uint32 r0, g0, b0;
					FB_DEVICE::device_pixel_type::unpack(pix0, r0, g0, b0);
					
					r += (uint8)(r0*bl);
					g += (uint8)(g0*bl);
					b += (uint8)(b0*bl);
				}
			}
			dbuf[y*SCREEN_WIDTH + x] = FB_DEVICE::device_pixel_type::pack(r, g, b);
		}
	}
}

//-------------------------------------------------------------
//turtle
void sample13(void)
{
	int len = 10;
	static double deg;
	static double da = 0.5;
	if (start) {
		deg = (rand() % 135) + 45;
		start = false;
	}

	ginfo->fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1);
	turtle.reset();
	for (int i=0; i <400; ++i) {
		turtle.move(len);
		turtle.turn(deg);
		++len;
	}
	MSLEEP(50);

	deg += da;
	if ( deg >= 180 ) {
		da = -da;
	}
	if ( deg < 45 ) {
		da = -da;
	}
}

//-------------------------------------------------------------
//reverse(up<->down)
void sample21(void)
{
	// initialize
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	static int32 y;
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		y = 0;
		start = false;
	}

	if (y >= SCREEN_HEIGHT) {
		oldt = 0;
		return;
	}

	int32 y_btm = (SCREEN_HEIGHT-1)-y;
	if ( y < SCREEN_HEIGHT/2 ) {
		// erase top
		ginfo->drawLine(0, y, SCREEN_WIDTH-1, y, BLACK);
		ginfo->drawLine(0, y+1, SCREEN_WIDTH-1, y+1, BLACK);
		// erase bottom
		ginfo->drawLine(0, y_btm, SCREEN_WIDTH-1, y_btm, BLACK);
		ginfo->drawLine(0, y_btm-1, SCREEN_WIDTH-1, y_btm-1, BLACK);
	} else {
		FB_DEVICE::pixel_type *ptr;
		// copy: bottom -> top
		ptr = &(buf[SCREEN_WIDTH * y_btm]);
		ginfo->putPattern(0, y, SCREEN_WIDTH, 1, ptr);
		ptr = &(buf[SCREEN_WIDTH * (y_btm-1)]);
		ginfo->putPattern(0, y+1, SCREEN_WIDTH, 1, ptr);
		// copy: top -> bottom 
		ptr = &(buf[SCREEN_WIDTH * y]);
		ginfo->putPattern(0, y_btm, SCREEN_WIDTH, 1, ptr);
		ptr = &(buf[SCREEN_WIDTH * (y+1)]);
		ginfo->putPattern(0, y_btm-1, SCREEN_WIDTH, 1, ptr);
	}

	y += 2;
}

//-------------------------------------------------------------
//reverse(left<->right)
void sample22(void)
{
	// initialize
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	static int32 x;
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		x = 0;
		start = false;
	}

	if (x >= SCREEN_WIDTH) {
		oldt = 0;
		return;
	}

	int32 x_right = (SCREEN_WIDTH-1)-x;
	if ( x < SCREEN_WIDTH/2 ) {
		// erase left 
		ginfo->drawLine(x, 0, x, SCREEN_HEIGHT-1, BLACK);
		ginfo->drawLine(x+1, 0, x+1, SCREEN_HEIGHT-1, BLACK);
		// erase right
		ginfo->drawLine(x_right, 0, x_right, SCREEN_HEIGHT-1, BLACK);
		ginfo->drawLine(x_right-1, 0, x_right-1, SCREEN_HEIGHT-1, BLACK);
	} else {
		FB_DEVICE::pixel_type *ptr;
		for ( int32 tmpy = 0 ; tmpy < SCREEN_HEIGHT ; ++tmpy ) {
			// copy: left -> right
			ptr = &(buf[(SCREEN_WIDTH * tmpy) + x_right]);
			ginfo->putPattern(x, tmpy, 1, 1, ptr);
			ptr = &(buf[(SCREEN_WIDTH * tmpy) + (x_right-1)]);
			ginfo->putPattern(x+1, tmpy, 1, 1, ptr);
			// copy: right -> left
			ptr = &(buf[(SCREEN_WIDTH * tmpy) + x]);
			ginfo->putPattern(x_right, tmpy, 1, 1, ptr);
			ptr = &(buf[(SCREEN_WIDTH * tmpy) + (x+1)]);
			ginfo->putPattern(x_right-1, tmpy, 1, 1, ptr);
		}
	}

	x += 2;
}

//-------------------------------------------------------------
//wipeout
void sample23(void)
{
	// initialize
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	static int32 x, y;
	static uint32 color;
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		x = 0;
		y = 0;
	//	color = rand_color();
		color = BLACK;
		start = false;
	}

	if ((x >= SCREEN_WIDTH/2) && (y >= SCREEN_HEIGHT/2)) {
		oldt = 0;
		return;
	}

	// erase
	if (y < SCREEN_HEIGHT/2) {
		int32 c_y = SCREEN_HEIGHT/2;
		// y: center->top
		ginfo->drawLine(0, (c_y-1)-y-1, SCREEN_WIDTH-1, c_y+y+1, color);
		ginfo->drawLine(0, (c_y-1)-y, SCREEN_WIDTH-1, c_y+y, color);
		// y: center->bottom
		ginfo->drawLine(0, c_y+y, SCREEN_WIDTH-1, (c_y-1)-y, color);
		ginfo->drawLine(0, c_y+y+1, SCREEN_WIDTH-1, (c_y-1)-y-1, color);
		y += 2;
	} else {
		// x: left->center
		ginfo->drawLine(x, 0, (SCREEN_WIDTH-1)-x, SCREEN_HEIGHT-1, color);
		ginfo->drawLine(x+1, 0, (SCREEN_WIDTH-1)-x-1, SCREEN_HEIGHT-1, color);
		// x: right->center
		ginfo->drawLine((SCREEN_WIDTH-1)-x, 0, x, SCREEN_HEIGHT-1, color);
		ginfo->drawLine((SCREEN_WIDTH-1)-x-1, 0, x+1, SCREEN_HEIGHT-1, color);
		x += 2;
	}
}

//-------------------------------------------------------------
//for test
void sample100(void)
{
	// initialize
	static FB_DEVICE::pixel_type buf[SCREEN_SIZE];
	static int32 y;
	if (start) {
		memcpy(buf, device->getBuffer(), SCREEN_SIZE * PIXEL_SIZE);
		y = 0;
		start = false;
	}

	if (y >= SCREEN_HEIGHT) {
		oldt = 0;
		return;
	}

	ginfo->drawLine(0, y, SCREEN_WIDTH-1, y, WHITE);
	ginfo->drawLine(0, y+1, SCREEN_WIDTH-1, y+1, BLACK);
	y += 2;
}
