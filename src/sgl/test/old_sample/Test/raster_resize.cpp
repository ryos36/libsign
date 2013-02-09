
#include "raster_resize.h"

/*!
	#define ENABLE_FLOAT_PNUM	浮動小数演算を使う場合に定義する

	固定小数版 … 浮動小数版よりわずかに画質が劣るが、処理速度は高速（デフォルト）
	浮動小数版 … 高画質だが、処理速度に難あり
*/
// #define ENABLE_FLOAT_PNUM


#ifndef ENABLE_FLOAT_PNUM
// 固定小数版向けの定義
#define PNUM_HALF					0x8000					// 0.5のこと
#define PNUM_ONE					0x10000					// 1.0のこと（つまり上位16bitを整数、下位16bitを）
#define _TO_FIXED_PNUM(x)			( (x) << 16 )			// 小数型へ変換
#define _FROM_FIXED_PNUM(x)			( (x) >> 16 )			// 小数型から元値に戻す
#define	_HALF_ADJUST(x)				( (x) + PNUM_HALF )		// 四捨五入（ 本当は _FLOOR((x) + PNUM_HALF) ）
#define	_FLOOR(x)					( (x) & 0xFFFF0000 )	// 小数点以下切捨て
#define _MODF(x,p)					( (x) & 0x0000FFFF )	// xの小数点部分の取り出し（pは整数部分）
#define	_HALF_ADJUST_ONLY_FPN(x)	( _HALF_ADJUST_COM(x) )	// 固定小数版の場合のみ元値に戻す処理
typedef int PNUM_POINT;				//!< 小数型（上位16bitを整数フィールド、下位16bitを小数フィールドとして扱う）

#else	// ENABLE_FLOAT_PNUM
// 浮動小数版向けの定義
#define PNUM_HALF					( 0.5 )
#define PNUM_ONE					( 1.0 )
#define _TO_FIXED_PNUM(x)			( (x) )
#define _FROM_FIXED_PNUM(x)			( (x) )
#define	_HALF_ADJUST(x)				( _FLOOR( (x)+PNUM_HALF ) )	// 四捨五入（整数型変換法）
#define	_FLOOR(x)					( (int)(x) )				// 小数点以下切捨て（整数型変換）
#define _MODF(x,p)					( (x) - ((PNUM_POINT)(p)) )	// xの小数点部分の取り出し（pは整数部分）
#define	_HALF_ADJUST_ONLY_FPN(x)	( (x) )
typedef float PNUM_POINT;			//!< 小数型

#endif	// ENABLE_FLOAT_PNUM

// 以下、両モード共通定義
#define	_HALF_ADJUST_COM(x)			( _FROM_FIXED_PNUM( _HALF_ADJUST(x) ) )	// 四捨五入（固定小数モードでは元値に戻す）
#define	_FLOOR_COM(x)				( _FROM_FIXED_PNUM( _FLOOR(x) ) )		// 小数点切捨て（固定小数モードでは元値に戻す）
#define _LINEAR(p,l,r)              ( ((PNUM_ONE-(p))*(l)) + ((p)*(r)) )	// リニア（一次補完）処理
#define _BILINEAR(p,q,ul,ur,dl,dr)  ( _LINEAR( q, _HALF_ADJUST_ONLY_FPN(_LINEAR(p,ul,ur)), \
                                      _HALF_ADJUST_ONLY_FPN(_LINEAR(p,dl,dr)) ) )	// バイリニア（双一次補完）処理


//------------------------------------------------------------------------------
/*!
	ニアレストネイバー法によるサイズ変換

	ニアレストネイバー法とは、変形後のあるピクセルが変形前にどこの座標に \n
	位置していたかを正確に計算し、算出した座標を四捨五入した座標のピクセル色を \n
	変形後の色として採用する方式

	@param	dst		変換後の画像データ情報のアドレス （ \n
					->addr	：変換後画像データの格納先アドレス (In/Out) \n
					->w		：変換したい画像の幅 (In) \n
					->h		：変換したい画像の高さ (In) ）
	@param	src		変換前の画像データ情報のアドレス （ \n
					->addr	：元画像データのアドレス (In) \n
					->w		：元画像の幅 (In) \n
					->h		：元画像の高さ (In) ）

	@note	座標計算のみで負荷は軽く、高速処理が可能 \n
			ただし、拡大率が大きくなると画質低下が目立つ \n
			画質を重視しないサムネイル表示には実用可能（？）
*/
void resizeByNearestneighbor( RASTER_INFO *dst, RASTER_INFO *src )
{
	int *dstBuff, *srcBuff;
	int dst_w, dst_h, src_w, src_h;
	PNUM_POINT wRatio, hRatio;
	int x, y;
	int i, j;

	dst_w = dst->w;
	dst_h = dst->h;
	src_w = src->w;
	src_h = src->h;

	// 逆変化率（dst画像座標 → src画像座標 の比例係数）を求める
	wRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_w) / (PNUM_POINT)dst_w;
	hRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_h) / (PNUM_POINT)dst_h;

	// 二アレストネイバー変換
	dstBuff = dst->addr;
	srcBuff = src->addr;
	for( i = 0; i < dst_h; i++ ) {
		y = _HALF_ADJUST_COM( i * hRatio );	// src画像座標系で最も近い y位置を取得
		for( j = 0; j < dst_w; j++ ){
			x = _HALF_ADJUST_COM( j * wRatio );	// src画像座標系で最も近い x位置を取得

			// src画像の最も適する画素値をそのままコピー
			*dstBuff++ = *(srcBuff + _GET_OFFSET( x, y, src_w ));
		}
	}
}

//------------------------------------------------------------------------------
/*!
	バイリニア法によるサイズ変換

	バイリニア法とは、変形後のあるピクセルが変形前のどの座標に相当するかを \n
	正確に計算し、その座標から見た上下左右4近傍のピクセルの色の平均値となる \n
	色データを作成し、変形後のピクセル色とする方式

	@param	dst		変換後の画像データ情報のアドレス （ \n
					->addr	：変換後画像データの格納先アドレス (In/Out) \n
					->w		：変換したい画像の幅 (In) \n
					->h		：変換したい画像の高さ (In) ）
	@param	src		変換前の画像データ情報のアドレス （ \n
					->addr	：元画像データのアドレス (In) \n
					->w		：元画像の幅 (In) \n
					->h		：元画像の高さ (In) ）

	@note	座標計算＋４つの色データの平均値計算が必要なため 処理負荷が高いが、 \n
			実用的な画質を保ったままの拡大・縮小が可能 \n
			固定小数版を使用することで、速度面でも実用的となる（？）
*/
void resizeByBilinear( RASTER_INFO *dst, RASTER_INFO *src )
{
	int *dstBuff, *srcBuff;
	int neighbor1, neighbor2, neighbor3, neighbor4;
	int r1, r2, r3, r4;
	int g1, g2, g3, g4;
	int b1, b2, b3, b4;
	int dst_w, dst_h, src_w, src_h;
	PNUM_POINT wRatio, hRatio;
	PNUM_POINT x, y;
	int floor_x, floor_y;
	int i, j;

	dst_w = dst->w;
	dst_h = dst->h;
	src_w = src->w;
	src_h = src->h;

	// 逆変化率（dst画像座標 → src画像座標 の比例係数）を求める
	wRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_w) / (PNUM_POINT)dst_w;
	hRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_h) / (PNUM_POINT)dst_h;

	// バイリニア変換
	dstBuff = dst->addr;
	srcBuff = src->addr;
	for( i = 0; i < dst_h; i++ ) {
		y = i * hRatio;				// src画像のy座標を正確に求める
		floor_y = _FLOOR_COM(y);	// dotレベルのy座標（小数点切捨て値）を求める
		y = _MODF( y, floor_y );	// 正確なy座標をfloor_yからのオフセットに変換
		for( j = 0; j < dst_w; j++ ){
			x = j * wRatio;				// src画像のx座標を正確に求める
			floor_x = _FLOOR_COM(x);	// dotレベルのx座標（小数点切捨て値）を求める
			x = _MODF( x, floor_x );	// 正確なx座標をfloor_xからのオフセットに変換

			// 4近傍の画素値を取得
			neighbor1 = *(srcBuff + _GET_OFFSET( floor_x, floor_y, src_w ));
			r1 = _GET_R(neighbor1), g1 = _GET_G(neighbor1), b1 = _GET_B(neighbor1);
			neighbor2 = *(srcBuff + _GET_OFFSET( floor_x + 1, floor_y, src_w ));
			r2 = _GET_R(neighbor2), g2 = _GET_G(neighbor2), b2 = _GET_B(neighbor2);
			neighbor3 = *(srcBuff + _GET_OFFSET( floor_x, floor_y + 1, src_w ));
			r3 = _GET_R(neighbor3), g3 = _GET_G(neighbor3), b3 = _GET_B(neighbor3);
			neighbor4 = *(srcBuff + _GET_OFFSET( floor_x + 1, floor_y + 1, src_w ));
			r4 = _GET_R(neighbor4), g4 = _GET_G(neighbor4), b4 = _GET_B(neighbor4);

			// 4近傍の画素値を線形補完して、dst画像の色を作成
			*dstBuff++ = _TO_R( _HALF_ADJUST_COM( _BILINEAR( x, y, r1, r2, r3, r4 ) ) ) |
						 _TO_G( _HALF_ADJUST_COM( _BILINEAR( x, y, g1, g2, g3, g4 ) ) ) |
						 _TO_B( _HALF_ADJUST_COM( _BILINEAR( x, y, b1, b2, b3, b4 ) ) );
		}
	}
}
