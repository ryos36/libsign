#ifndef __RASTER_RESIZE_H__
#define __RASTER_RESIZE_H__

// �F�f�[�^32bit����RGB�\���̒�`
#define _COLOR_MASK(x)				( (x) & 0xFF )
#define _GET_R(r)					( _COLOR_MASK(r) )
#define _GET_G(g) 					( _COLOR_MASK((g) >>  8) )
#define _GET_B(b) 					( _COLOR_MASK((b) >> 16) )
#define _TO_R(r)					( (int)(r) )
#define _TO_G(g)					( ((int)(g)) << 8 )
#define _TO_B(b)					( ((int)(b)) << 16 )
// (x,y)���W�̐F�f�[�^�ւ̃I�t�Z�b�g�Z�o
#define	_GET_OFFSET(x,y,w)			( (x) + ((y)*(w)) )

//! �摜�f�[�^���
typedef struct {
	int *addr;	//!< KELLY�d�l�i1dot=32bit(ABGR)�j�̉摜�f�[�^�z��̐擪�A�h���X
	int w;		//!< �摜�̕�
	int h;		//!< �摜�̍���
} RASTER_INFO;

// �T�C�Y�ϊ�API
extern void resizeByNearestneighbor( RASTER_INFO *dst, RASTER_INFO *src );
extern void resizeByBilinear( RASTER_INFO *dst, RASTER_INFO *src );

#endif //__RASTER_RESIZE_H__
