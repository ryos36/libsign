#include "FontTest.h"
#include "TestDevice.h"
#include <gl/glut.h>
#include <cmath>

using namespace sign::sgl;

extern TestDevice *pDevice;
static int wait = 500;

#define SHOW()	glutPostRedisplay(); \
				::Sleep(wait) \

#define CLEAR()	pDevice->clearScreen()


void
FontTest::setUp()
{
}

void
FontTest::tearDown()
{
}


void 
FontTest::test_render()
{
}

