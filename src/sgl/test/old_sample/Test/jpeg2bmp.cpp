#include "jpeg2bmp.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "jpeglib.h"

//set_jpeg_data i/f
void  my_src (j_decompress_ptr dinfo, JOCTET * address, size_t length);
}


//jpegData ... Jpegファイルのバイトストリーム
int
jpeg2bmp( RASTER_INFO *bmp, const unsigned long *jpegData, const unsigned long size )
{
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW jsamprow[MAX_SCANLINES];
	int	*pData;
	JDIMENSION lines;
	int i, j;

	// 初期化
	dinfo.err = jpeg_std_error( &jerr );
	jpeg_create_decompress( &dinfo );

	// データセット
	my_src( &dinfo, (JOCTET *)jpegData, (size_t)size );

	// ヘッダ読み込み
	jpeg_read_header( &dinfo, TRUE );

	// 解像度制限
	if( dinfo.output_width * dinfo.output_height > INPUT_SIZE_MAX ){
		return 1;
	}

	// 読み込み開始準備
	/* NIY 読み込みを早くする設定
	       （とりあえず色々試したが、以下の３つで十分に思えた。
	         速度落とす＝画質劣化 なので、高速設定し過ぎるのも問題となる） */
	dinfo.dct_method = JDCT_IFAST;
	dinfo.do_block_smoothing = FALSE;
	dinfo.do_fancy_upsampling = FALSE;
	// 縮小率の設定（scale_num/scale_denom）
	dinfo.scale_num = 1;
	dinfo.scale_denom = 1;

#ifdef _sokudo_amari_kawaranai
	dinfo.buffered_image = FALSE;
	dinfo.raw_data_out = FALSE;
	dinfo.two_pass_quantize = FALSE;
	dinfo.dither_mode = JDITHER_ORDERED;
	dinfo.quantize_colors = FALSE;
	if( !dinfo.quantize_colors )	/* don't override an earlier -colors */
		dinfo.desired_number_of_colors = 216;

	dinfo.enable_1pass_quant = FALSE;
	dinfo.enable_external_quant = FALSE;
	dinfo.enable_2pass_quant = FALSE;
#endif

#ifdef _fast_option
	dinfo.two_pass_quantize = FALSE;
	dinfo.dither_mode = JDITHER_ORDERED;
	if (! dinfo.quantize_colors) /* don't override an earlier -colors */
		dinfo.desired_number_of_colors = 216;
	dinfo.dct_method = JDCT_FASTEST;
	dinfo.do_fancy_upsampling = FALSE;
#endif

	jpeg_start_decompress( &dinfo );

	// 読み込み
	for( i = 0; i < MAX_SCANLINES; i++ ){
		jsamprow[i] = (JSAMPROW)malloc( 3 * dinfo.output_width );
		while( jsamprow[i] == NULL );
	}
	pData = bmp->addr;
	while( dinfo.output_scanline < dinfo.output_height ) {
		lines = dinfo.output_scanline;
		jpeg_read_scanlines( &dinfo, jsamprow, MAX_SCANLINES );

		// 変換
		lines = dinfo.output_scanline - lines;
		for( i = 0; i < lines; i++ ){
			for( j = 0; j < dinfo.output_width; j++ ){
				*pData++ = _TO_R(jsamprow[i][j*3]) | _TO_G(jsamprow[i][j*3+1]) | _TO_B(jsamprow[i][j*3+2]);
			}
		}
	}

	bmp->w = dinfo.output_width;
	bmp->h = dinfo.output_height;

	// オブジェクト解放
	for( i = 0; i < MAX_SCANLINES; i++ ){
		free( jsamprow[i] );
	}
	jpeg_finish_decompress( &dinfo );
	jpeg_destroy_decompress( &dinfo );

	return 0;
}