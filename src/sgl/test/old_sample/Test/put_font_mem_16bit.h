#ifndef __KELLY_K2_NMCS_SUPPORT_H__
#define __KELLY_K2_NMCS_SUPPORT_H__

#ifdef __cplusplus
extern "C" {
#endif


//矩形構造体。
typedef struct _CRect
{
	int x;
	int y;
	int width;
	int height;
} CRect;


//RGBカラー構造体。各値は 0-255 の範囲で指定する。
typedef struct _CColor
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} CColor;

//!------------------------------------------
/*
	@param[in] addr   メモリ領域のアドレス。使用する側で確保する必要がある。
	@param[in] bitMap 描画結果の矩形を指定する
	@param[in] clip   描画可能な矩形を指定する bitMap矩形はこの矩形からはみ出してはならない
	@param[in] str    描画する文字を指定する。マルチバイト文字列で文字コードはshift-jisであること。
	@param[in] font   フォントの矩形を指定する
	@param[in] color  描画する色を指定する
	@return           描画した文字のバイト数
*/
int
put_font_mem_16bit( unsigned short *addr, CRect *bitMap, CRect *clip, char *str, CRect *_font, CColor *color );

#ifdef __cplusplus
} //extern "C" 
#endif

#endif //__KELLY_K2_NMCS_SUPPORT_H__

