#ifndef __JPEG_2_BMP_h__
#define __JPEG_2_BMP_h__

#include "raster_resize.h"

#define INPUT_HEIGHT    800
#define INPUT_WIDTH     600
#define INPUT_SIZE_MAX  (INPUT_HEIGHT * INPUT_WIDTH)    // about 800x600 

#define MAX_SCANLINES	2
int jpeg2bmp( RASTER_INFO *bmp, const unsigned long *jpegData, const unsigned long size );

#endif //__JPEG_2_BMP_h__

