#ifndef __SIGN_SGL_DEVICE_PIXEL_TEST_H__
#define __SIGN_SGL_DEVICE_PIXEL_TEST_H__

#include <cppunit/TestCase.h>

class DevicePixelTest : public CppUnit::TestCase
{
public:
	DevicePixelTest(){}
	~DevicePixelTest(){}

	virtual void setUp();
	virtual void tearDown();
	
	void test_alphaBlend();
	void test_R5G5B5();
	void test_R5G6B5();
	void test_R8G8B8A8();
	void test_A8B8G8R8();
};

#endif //__SIGN_SGL_DEVICE_PIXEL_TEST_H__
