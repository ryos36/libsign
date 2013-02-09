/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"
#include "TestDevice.h"
#include <windows.h>
#include <GL/glut.h>

using namespace sign::sgl;



static int argc = 1;
static char *argv[] = { "OpenGL" };
TestDevice::TestDevice()
: OpenGLDevice( "TestDevice", SCREEN_WIDTH, SCREEN_HEIGHT, argc, argv )
{
	pBuffer = buf;
}

void 
TestDevice::clearScreen()
{
	memset( buf, 0, bsize );
}

