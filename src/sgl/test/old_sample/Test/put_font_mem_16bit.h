#ifndef __KELLY_K2_NMCS_SUPPORT_H__
#define __KELLY_K2_NMCS_SUPPORT_H__

#ifdef __cplusplus
extern "C" {
#endif


//��`�\���́B
typedef struct _CRect
{
	int x;
	int y;
	int width;
	int height;
} CRect;


//RGB�J���[�\���́B�e�l�� 0-255 �͈̔͂Ŏw�肷��B
typedef struct _CColor
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} CColor;

//!------------------------------------------
/*
	@param[in] addr   �������̈�̃A�h���X�B�g�p���鑤�Ŋm�ۂ���K�v������B
	@param[in] bitMap �`�挋�ʂ̋�`���w�肷��
	@param[in] clip   �`��\�ȋ�`���w�肷�� bitMap��`�͂��̋�`����͂ݏo���Ă͂Ȃ�Ȃ�
	@param[in] str    �`�悷�镶�����w�肷��B�}���`�o�C�g������ŕ����R�[�h��shift-jis�ł��邱�ƁB
	@param[in] font   �t�H���g�̋�`���w�肷��
	@param[in] color  �`�悷��F���w�肷��
	@return           �`�悵�������̃o�C�g��
*/
int
put_font_mem_16bit( unsigned short *addr, CRect *bitMap, CRect *clip, char *str, CRect *_font, CColor *color );

#ifdef __cplusplus
} //extern "C" 
#endif

#endif //__KELLY_K2_NMCS_SUPPORT_H__

