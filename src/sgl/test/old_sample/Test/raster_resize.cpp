
#include "raster_resize.h"

/*!
	#define ENABLE_FLOAT_PNUM	�����������Z���g���ꍇ�ɒ�`����

	�Œ菬���� �c ���������ł��킸���ɉ掿����邪�A�������x�͍����i�f�t�H���g�j
	���������� �c ���掿�����A�������x�ɓ��
*/
// #define ENABLE_FLOAT_PNUM


#ifndef ENABLE_FLOAT_PNUM
// �Œ菬���Ō����̒�`
#define PNUM_HALF					0x8000					// 0.5�̂���
#define PNUM_ONE					0x10000					// 1.0�̂��Ɓi�܂���16bit�𐮐��A����16bit���j
#define _TO_FIXED_PNUM(x)			( (x) << 16 )			// �����^�֕ϊ�
#define _FROM_FIXED_PNUM(x)			( (x) >> 16 )			// �����^���猳�l�ɖ߂�
#define	_HALF_ADJUST(x)				( (x) + PNUM_HALF )		// �l�̌ܓ��i �{���� _FLOOR((x) + PNUM_HALF) �j
#define	_FLOOR(x)					( (x) & 0xFFFF0000 )	// �����_�ȉ��؎̂�
#define _MODF(x,p)					( (x) & 0x0000FFFF )	// x�̏����_�����̎��o���ip�͐��������j
#define	_HALF_ADJUST_ONLY_FPN(x)	( _HALF_ADJUST_COM(x) )	// �Œ菬���ł̏ꍇ�̂݌��l�ɖ߂�����
typedef int PNUM_POINT;				//!< �����^�i���16bit�𐮐��t�B�[���h�A����16bit�������t�B�[���h�Ƃ��Ĉ����j

#else	// ENABLE_FLOAT_PNUM
// ���������Ō����̒�`
#define PNUM_HALF					( 0.5 )
#define PNUM_ONE					( 1.0 )
#define _TO_FIXED_PNUM(x)			( (x) )
#define _FROM_FIXED_PNUM(x)			( (x) )
#define	_HALF_ADJUST(x)				( _FLOOR( (x)+PNUM_HALF ) )	// �l�̌ܓ��i�����^�ϊ��@�j
#define	_FLOOR(x)					( (int)(x) )				// �����_�ȉ��؎̂āi�����^�ϊ��j
#define _MODF(x,p)					( (x) - ((PNUM_POINT)(p)) )	// x�̏����_�����̎��o���ip�͐��������j
#define	_HALF_ADJUST_ONLY_FPN(x)	( (x) )
typedef float PNUM_POINT;			//!< �����^

#endif	// ENABLE_FLOAT_PNUM

// �ȉ��A�����[�h���ʒ�`
#define	_HALF_ADJUST_COM(x)			( _FROM_FIXED_PNUM( _HALF_ADJUST(x) ) )	// �l�̌ܓ��i�Œ菬�����[�h�ł͌��l�ɖ߂��j
#define	_FLOOR_COM(x)				( _FROM_FIXED_PNUM( _FLOOR(x) ) )		// �����_�؎̂āi�Œ菬�����[�h�ł͌��l�ɖ߂��j
#define _LINEAR(p,l,r)              ( ((PNUM_ONE-(p))*(l)) + ((p)*(r)) )	// ���j�A�i�ꎟ�⊮�j����
#define _BILINEAR(p,q,ul,ur,dl,dr)  ( _LINEAR( q, _HALF_ADJUST_ONLY_FPN(_LINEAR(p,ul,ur)), \
                                      _HALF_ADJUST_ONLY_FPN(_LINEAR(p,dl,dr)) ) )	// �o�C���j�A�i�o�ꎟ�⊮�j����


//------------------------------------------------------------------------------
/*!
	�j�A���X�g�l�C�o�[�@�ɂ��T�C�Y�ϊ�

	�j�A���X�g�l�C�o�[�@�Ƃ́A�ό`��̂���s�N�Z�����ό`�O�ɂǂ��̍��W�� \n
	�ʒu���Ă������𐳊m�Ɍv�Z���A�Z�o�������W���l�̌ܓ��������W�̃s�N�Z���F�� \n
	�ό`��̐F�Ƃ��č̗p�������

	@param	dst		�ϊ���̉摜�f�[�^���̃A�h���X �i \n
					->addr	�F�ϊ���摜�f�[�^�̊i�[��A�h���X (In/Out) \n
					->w		�F�ϊ��������摜�̕� (In) \n
					->h		�F�ϊ��������摜�̍��� (In) �j
	@param	src		�ϊ��O�̉摜�f�[�^���̃A�h���X �i \n
					->addr	�F���摜�f�[�^�̃A�h���X (In) \n
					->w		�F���摜�̕� (In) \n
					->h		�F���摜�̍��� (In) �j

	@note	���W�v�Z�݂̂ŕ��ׂ͌y���A�����������\ \n
			�������A�g�嗦���傫���Ȃ�Ɖ掿�ቺ���ڗ��� \n
			�掿���d�����Ȃ��T���l�C���\���ɂ͎��p�\�i�H�j
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

	// �t�ω����idst�摜���W �� src�摜���W �̔��W���j�����߂�
	wRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_w) / (PNUM_POINT)dst_w;
	hRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_h) / (PNUM_POINT)dst_h;

	// ��A���X�g�l�C�o�[�ϊ�
	dstBuff = dst->addr;
	srcBuff = src->addr;
	for( i = 0; i < dst_h; i++ ) {
		y = _HALF_ADJUST_COM( i * hRatio );	// src�摜���W�n�ōł��߂� y�ʒu���擾
		for( j = 0; j < dst_w; j++ ){
			x = _HALF_ADJUST_COM( j * wRatio );	// src�摜���W�n�ōł��߂� x�ʒu���擾

			// src�摜�̍ł��K�����f�l�����̂܂܃R�s�[
			*dstBuff++ = *(srcBuff + _GET_OFFSET( x, y, src_w ));
		}
	}
}

//------------------------------------------------------------------------------
/*!
	�o�C���j�A�@�ɂ��T�C�Y�ϊ�

	�o�C���j�A�@�Ƃ́A�ό`��̂���s�N�Z�����ό`�O�̂ǂ̍��W�ɑ������邩�� \n
	���m�Ɍv�Z���A���̍��W���猩���㉺���E4�ߖT�̃s�N�Z���̐F�̕��ϒl�ƂȂ� \n
	�F�f�[�^���쐬���A�ό`��̃s�N�Z���F�Ƃ������

	@param	dst		�ϊ���̉摜�f�[�^���̃A�h���X �i \n
					->addr	�F�ϊ���摜�f�[�^�̊i�[��A�h���X (In/Out) \n
					->w		�F�ϊ��������摜�̕� (In) \n
					->h		�F�ϊ��������摜�̍��� (In) �j
	@param	src		�ϊ��O�̉摜�f�[�^���̃A�h���X �i \n
					->addr	�F���摜�f�[�^�̃A�h���X (In) \n
					->w		�F���摜�̕� (In) \n
					->h		�F���摜�̍��� (In) �j

	@note	���W�v�Z�{�S�̐F�f�[�^�̕��ϒl�v�Z���K�v�Ȃ��� �������ׂ��������A \n
			���p�I�ȉ掿��ۂ����܂܂̊g��E�k�����\ \n
			�Œ菬���ł��g�p���邱�ƂŁA���x�ʂł����p�I�ƂȂ�i�H�j
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

	// �t�ω����idst�摜���W �� src�摜���W �̔��W���j�����߂�
	wRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_w) / (PNUM_POINT)dst_w;
	hRatio = (PNUM_POINT)_TO_FIXED_PNUM(src_h) / (PNUM_POINT)dst_h;

	// �o�C���j�A�ϊ�
	dstBuff = dst->addr;
	srcBuff = src->addr;
	for( i = 0; i < dst_h; i++ ) {
		y = i * hRatio;				// src�摜��y���W�𐳊m�ɋ��߂�
		floor_y = _FLOOR_COM(y);	// dot���x����y���W�i�����_�؎̂Ēl�j�����߂�
		y = _MODF( y, floor_y );	// ���m��y���W��floor_y����̃I�t�Z�b�g�ɕϊ�
		for( j = 0; j < dst_w; j++ ){
			x = j * wRatio;				// src�摜��x���W�𐳊m�ɋ��߂�
			floor_x = _FLOOR_COM(x);	// dot���x����x���W�i�����_�؎̂Ēl�j�����߂�
			x = _MODF( x, floor_x );	// ���m��x���W��floor_x����̃I�t�Z�b�g�ɕϊ�

			// 4�ߖT�̉�f�l���擾
			neighbor1 = *(srcBuff + _GET_OFFSET( floor_x, floor_y, src_w ));
			r1 = _GET_R(neighbor1), g1 = _GET_G(neighbor1), b1 = _GET_B(neighbor1);
			neighbor2 = *(srcBuff + _GET_OFFSET( floor_x + 1, floor_y, src_w ));
			r2 = _GET_R(neighbor2), g2 = _GET_G(neighbor2), b2 = _GET_B(neighbor2);
			neighbor3 = *(srcBuff + _GET_OFFSET( floor_x, floor_y + 1, src_w ));
			r3 = _GET_R(neighbor3), g3 = _GET_G(neighbor3), b3 = _GET_B(neighbor3);
			neighbor4 = *(srcBuff + _GET_OFFSET( floor_x + 1, floor_y + 1, src_w ));
			r4 = _GET_R(neighbor4), g4 = _GET_G(neighbor4), b4 = _GET_B(neighbor4);

			// 4�ߖT�̉�f�l����`�⊮���āAdst�摜�̐F���쐬
			*dstBuff++ = _TO_R( _HALF_ADJUST_COM( _BILINEAR( x, y, r1, r2, r3, r4 ) ) ) |
						 _TO_G( _HALF_ADJUST_COM( _BILINEAR( x, y, g1, g2, g3, g4 ) ) ) |
						 _TO_B( _HALF_ADJUST_COM( _BILINEAR( x, y, b1, b2, b3, b4 ) ) );
		}
	}
}
