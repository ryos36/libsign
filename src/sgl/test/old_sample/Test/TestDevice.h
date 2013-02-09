/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_TEST_DEVICE_H__
#define __SIGN_SGL_TEST_DEVICE_H__

#include "sign/sign_types.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/OpenGLDevice.h"


class TestDevice 
	: public sign::sgl::OpenGLDevice
{
public:
	enum
	{
		SCREEN_WIDTH = 800,
		SCREEN_HEIGHT = 600,
	};

public:
	TestDevice();
	~TestDevice(){}

	void clearScreen();

	pixel_type *pBuffer;
};


#endif // __SIGN_SGL_TEST_DEVICE_H__
