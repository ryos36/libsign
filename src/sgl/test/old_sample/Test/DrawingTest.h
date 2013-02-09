#ifndef __SIGN_SGL_DRAWING_TEST_H__
#define __SIGN_SGL_DRAWING_TEST_H__

#include <cppunit/TestCase.h>

class DrawingTest : public CppUnit::TestCase
{
public:
	DrawingTest(){}
	~DrawingTest(){}

	virtual void setUp();
	virtual void tearDown();
	
	void test_setPixel();
	void test_drawLine1();
	void test_drawLine2();
	void test_fillRectangle();

	void test_putPattern8bpp();
	void test_showGIF();

	void test_overrun();
};

#endif //__SIGN_SGL_DRAWING_TEST_H__
