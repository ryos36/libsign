#ifndef __SIGN_SGL_FONT_TEST_H__
#define __SIGN_SGL_FONT_TEST_H__

#include <cppunit/TestCase.h>

class FontTest : public CppUnit::TestCase
{
public:
	FontTest(){}
	~FontTest(){}

	virtual void setUp();
	virtual void tearDown();
	
	void test_render();

};

#endif //__SIGN_SGL_DRAWING_TEST_H__
