
//-------------------------------------------------------------
#include "put_font_mem_16bit.h"
#include "sign/sgl++/sgl.h"
#include "sign/sgl++/FrameBufferDevice.h"
#include "sign/sgl++/DevicePixel.h"
#include "sign/sgl++/DefaultGraphicDeviceManager.h"
#include "sign/sgl++/DefaultFontManager.h"
#include "sign/sgl++/fonts/kelly_lim_font.h"


//-------------------------------------------------------------
//16ビットのメモリ(R5G5B5)に対して描画するデバイスクラス
class NMCSMemoryDevice
	: public sign::sgl::FrameBufferDevice< sign::sgl::DevicePixel_R5G5B5 >
{
public:
	NMCSMemoryDevice()
	: FrameBufferDevice< sign::sgl::DevicePixel_R5G5B5 > ( "", ZERO_P_LEFT_UP, ANGLE_R000, STATUS_HAS_XINC | STATUS_HAS_YINC | STATUS_HAS_X2Y , 
					0, 0, 0, 0, 0, 0, buf )
	{}

	void setFrameBuffer( sign::sgl::DevicePixel_R5G5B5::pixel_type *buf, int32 width, int32 height )
	{
		this->buf = buf;
		maxSizeX = viewSizeX = width;
		maxSizeY = viewSizeY = height;
		bsize = maxSizeX * maxSizeY * sizeof(pixel_type);
	}

	virtual void repaint(void){};
	virtual void backlight(bool on){};
};

//-------------------------------------------------------------
static sign::sgl::GraphicInfo *pGinfo;
static NMCSMemoryDevice *pDevice;

//-------------------------------------------------------------
//!初期化用オブジェクト
class NMCSMemoryDeviceInitializer
{
public:
	NMCSMemoryDeviceInitializer::NMCSMemoryDeviceInitializer()
	{
		using namespace sign::sgl;

		static NMCSMemoryDevice device;
		pDevice = &device;

		static DefaultGraphicDeviceManager deviceManager;
		deviceManager.setDevice( &device );
		GraphicInfo::setGraphicDeviceManager( &deviceManager );
		
		static DefaultFontManager fontManager;
		fontManager.setFontTable(&KellyLimFontTable);
		GraphicInfo::setFontManager( &fontManager );
		

		static GraphicInfo ginfo("");
		pGinfo = &ginfo;
	}
};

namespace {
	NMCSMemoryDeviceInitializer initializer;
}






	
//-------------------------------------------------------------
int
put_font_mem_16bit( unsigned short *addr, CRect *bitMap, CRect *clip, char *str, CRect *_font, CColor *color )
{
	using namespace sign::sgl;

	int char_bytes = 0;

	//文字のバイト数を判定
	int code;
	char c_str[4];
	if ( isShiftJisMultiByteChar(*str) ) {
		char_bytes = 2;
		code = ( (unsigned char)str[1] | ( (unsigned char)str[0] << 8 ) );
		c_str[0] = str[0];
		c_str[1] = str[1];
		c_str[2] = '\0';
	}
	else {
		char_bytes = 1;
		code = (unsigned char)str[0];
		c_str[0] = str[0];
		c_str[1] = '\0';
	}

	
	pDevice->setFrameBuffer( addr, bitMap->width, bitMap->height );

	//文字種の判定 NIY
	// if ( isXXX(code) ) ...

	//文字列描画のクリッピング NIY (drawStringをオーバーロード)
	pGinfo->drawString( _font->x, _font->y, _font->width, _font->height, c_str, 
		DevicePixel_R5G5B5::pack( color->red, color->green ,color->blue ), 1, 0, 0 );
	
	return char_bytes;
}


