#ifndef __RASTER_RESIZE_H__
#define __RASTER_RESIZE_H__

// 色データ32bit中のRGB構成の定義
#define _COLOR_MASK(x)				( (x) & 0xFF )
#define _GET_R(r)					( _COLOR_MASK(r) )
#define _GET_G(g) 					( _COLOR_MASK((g) >>  8) )
#define _GET_B(b) 					( _COLOR_MASK((b) >> 16) )
#define _TO_R(r)					( (int)(r) )
#define _TO_G(g)					( ((int)(g)) << 8 )
#define _TO_B(b)					( ((int)(b)) << 16 )
// (x,y)座標の色データへのオフセット算出
#define	_GET_OFFSET(x,y,w)			( (x) + ((y)*(w)) )

//! 画像データ情報
typedef struct {
	int *addr;	//!< KELLY仕様（1dot=32bit(ABGR)）の画像データ配列の先頭アドレス
	int w;		//!< 画像の幅
	int h;		//!< 画像の高さ
} RASTER_INFO;

// サイズ変換API
extern void resizeByNearestneighbor( RASTER_INFO *dst, RASTER_INFO *src );
extern void resizeByBilinear( RASTER_INFO *dst, RASTER_INFO *src );

#endif //__RASTER_RESIZE_H__
