#ifndef __SIGN_SGL_DRAW_STRING_TEST_H__
#define __SIGN_SGL_DRAW_STRING_TEST_H__

#include <cppunit/TestCase.h>

class DrawStringTest : public CppUnit::TestCase
{
public:
	DrawStringTest(){}
	~DrawStringTest(){}

	virtual void setUp();
	virtual void tearDown();
	
	void test_drawString1();
	void test_drawString2();
	void test_drawString_size1();
	void test_drawString_size2();
	void test_drawString_alpha1();
	void test_drawString_alpha2();
	void test_drawString_clip1();
	void test_drawString_clip2();

	void test_drawString_wchar1();
	void test_drawString_wchar2();

	void test_drawString_wchar_clip1();
	void test_drawString_wchar_clip2();

	void test_measureString();
	void test_measureString_wchar();

};

#endif //__SIGN_SGL_DRAW_STRING_TEST_H__
