#include "jpeg2bmp.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "jpeglib.h"

//set_jpeg_data i/f
void  my_src (j_decompress_ptr dinfo, JOCTET * address, size_t length);
}


//jpegData ... Jpeg�t�@�C���̃o�C�g�X�g���[��
int
jpeg2bmp( RASTER_INFO *bmp, const unsigned long *jpegData, const unsigned long size )
{
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW jsamprow[MAX_SCANLINES];
	int	*pData;
	JDIMENSION lines;
	int i, j;

	// ������
	dinfo.err = jpeg_std_error( &jerr );
	jpeg_create_decompress( &dinfo );

	// �f�[�^�Z�b�g
	my_src( &dinfo, (JOCTET *)jpegData, (size_t)size );

	// �w�b�_�ǂݍ���
	jpeg_read_header( &dinfo, TRUE );

	// �𑜓x����
	if( dinfo.output_width * dinfo.output_height > INPUT_SIZE_MAX ){
		return 1;
	}

	// �ǂݍ��݊J�n����
	/* NIY �ǂݍ��݂𑁂�����ݒ�
	       �i�Ƃ肠�����F�X���������A�ȉ��̂R�ŏ\���Ɏv�����B
	         ���x���Ƃ����掿�� �Ȃ̂ŁA�����ݒ肵�߂���̂����ƂȂ�j */
	dinfo.dct_method = JDCT_IFAST;
	dinfo.do_block_smoothing = FALSE;
	dinfo.do_fancy_upsampling = FALSE;
	// �k�����̐ݒ�iscale_num/scale_denom�j
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

	// �ǂݍ���
	for( i = 0; i < MAX_SCANLINES; i++ ){
		jsamprow[i] = (JSAMPROW)malloc( 3 * dinfo.output_width );
		while( jsamprow[i] == NULL );
	}
	pData = bmp->addr;
	while( dinfo.output_scanline < dinfo.output_height ) {
		lines = dinfo.output_scanline;
		jpeg_read_scanlines( &dinfo, jsamprow, MAX_SCANLINES );

		// �ϊ�
		lines = dinfo.output_scanline - lines;
		for( i = 0; i < lines; i++ ){
			for( j = 0; j < dinfo.output_width; j++ ){
				*pData++ = _TO_R(jsamprow[i][j*3]) | _TO_G(jsamprow[i][j*3+1]) | _TO_B(jsamprow[i][j*3+2]);
			}
		}
	}

	bmp->w = dinfo.output_width;
	bmp->h = dinfo.output_height;

	// �I�u�W�F�N�g���
	for( i = 0; i < MAX_SCANLINES; i++ ){
		free( jsamprow[i] );
	}
	jpeg_finish_decompress( &dinfo );
	jpeg_destroy_decompress( &dinfo );

	return 0;
}