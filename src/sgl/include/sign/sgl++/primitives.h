/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_PRIMITIVES_H__
#define __SIGN_SGL_PRIMITIVES_H__

#include "sign/sign_types.h"
namespace sign {
namespace sgl {

#undef max
#undef min

//-----------------------------------------------------
//Packed 32bit version 0xAABBGGRR
//TODO:テンプレートクラス
#define R_BITS 8
#define G_BITS 8
#define B_BITS 8
#define A_BITS 8
#define R_ALIGN 0
#define G_ALIGN 8
#define B_ALIGN 16
#define A_ALIGN 24

#define R_MASK ( ( (1 << R_BITS) - 1) << R_ALIGN )
#define G_MASK ( ( (1 << G_BITS) - 1) << G_ALIGN )
#define B_MASK ( ( (1 << B_BITS) - 1) << B_ALIGN )
#define A_MASK ( ( (1 << A_BITS) - 1) << A_ALIGN )

struct RGBAColor
{
	enum {
		ALPHA_BLEND_0  = 0,
		ALPHA_BLEND_10 = 26,
		ALPHA_BLEND_20 = 51,
		ALPHA_BLEND_30 = 77,
		ALPHA_BLEND_40 = 102,
		ALPHA_BLEND_50 = 128,
		ALPHA_BLEND_60 = 153,
		ALPHA_BLEND_70 = 179,
		ALPHA_BLEND_80 = 204,
		ALPHA_BLEND_90 = 230,
		ALPHA_BLEND_100 = 255,

		ALPHA_BLEND_25 = 64,
		ALPHA_BLEND_75 = 191,
	};

	RGBAColor() 
		: color(0)
	{}

	RGBAColor( uint8 r, uint8 g, uint8 b, uint8 a = ALPHA_BLEND_100 ) 
	{
		color = (a<<A_ALIGN) | (b<<B_ALIGN) | (g<<G_ALIGN) | (r<<R_ALIGN);
	}

	RGBAColor& operator = (const RGBAColor& newColor)
	{
		color = newColor.color;
		return *this; 
	}

	RGBAColor& operator += ( int v ) 
	{
		int32 r = getR() + v;
		if ( r > 0xFF ) r = 0xFF;
		setR( r );
		
		int32 g = getG() + v;
		if ( g > 0xFF ) g = 0xFF;
		setG( g );

		int32 b = getB() + v;
		if ( b > 0xFF ) b = 0xFF;
		setB( b );
		
		return *this;
	}

	RGBAColor& operator += ( const RGBAColor& other ) 
	{
		int32 r = getR() + other.getR();
		if ( r > 0xFF ) r = 0xFF;
		setR( r );

		int32 g = getG() + other.getG();
		if ( g > 0xFF ) g = 0xFF;
		setG( g );

		int32 b = getB() + other.getB();
		if ( b > 0xFF ) b = 0xFF;
		setB( b );
		
		return *this;
	}

	RGBAColor& operator -= ( int v ) 
	{
		int32 r = getR() - v;
		if ( r < 0 ) r = 0;
		setR( r );

		int32 g = getG() - v;
		if ( g < 0 ) g = 0;
		setG( g );

		int32 b = getB() - v;
		if ( b < 0 ) b = 0;
		setB( b );

		return *this;
	}

	RGBAColor& operator -= ( const RGBAColor& other ) 
	{
		int32 r = getR() - other.getR();
		if ( r < 0 ) r = 0;
		setR( r );

		int32 g = getG() + other.getG();
		if ( g < 0 ) g = 0;
		setG( g );

		int32 b = getB() + other.getB();
		if ( b < 0 ) b = 0;
		setB( b );

		return *this;
	}

	RGBAColor operator + ( int v ) const
	{
		RGBAColor c(*this);
		c += v;
		return c;
	}

	RGBAColor operator - ( int v ) const
	{
		RGBAColor c(*this);
		c -= v;
		return c;
	}

	RGBAColor operator ~ ()
	{
		RGBAColor c;
		c.color = ~color;
		return c;
	}

	bool operator == ( const RGBAColor &other ) const
	{ 
		return (color == other.color);
	}
	
	bool operator != ( const RGBAColor &other ) const 
	{ 
		return (color != other.color);
	}

	uint32 rgb2Color16() const
	{
		uint32 r = ((color & R_MASK) >> R_ALIGN );
		uint32 g = ((color & G_MASK) >> G_ALIGN );
		uint32 b = ((color & B_MASK) >> B_ALIGN );

		return ((r&0xF8)<<8) | ((g&0xF8)<<3) | ((b&0xF8)>>3);
	}

	uint32 rgb2PixelValue() const
	{
		return color;
	}

	//setter & getter
	inline void setR( uint8 r )
	{
		color = (color & (~R_MASK)) | (r<<R_ALIGN);
	}

	inline uint8 getR() const
	{
		return ((color & R_MASK) >> R_ALIGN);
	}

	inline void setG( uint8 g )
	{
		color = (color & (~G_MASK)) | (g<<G_ALIGN);
	}

	inline uint8 getG() const
	{
		return ((color & G_MASK) >> G_ALIGN);
	}

	inline void setB( uint8 b )
	{
		color = (color & (~B_MASK)) | (b<<B_ALIGN);
	}

	inline uint8 getB() const
	{
		return ((color & B_MASK) >> B_ALIGN);
	}

	inline void setA( uint8 a )
	{
		color = (color & (~A_MASK)) | (a<<A_ALIGN);
	}

	inline uint8 getA() const
	{
		return ((color & (uint32)A_MASK) >> A_ALIGN);
	}

	static RGBAColor createRGBColor(uint32 rgbcolor) {
		RGBAColor bgrcolor;

		bgrcolor.color = 
			(( rgbcolor & 0x000000FF ) << 16) | 
			(( rgbcolor & 0x0000FF00 )      ) | 
			(( rgbcolor & 0x00FF0000 ) >> 16);

		
		return bgrcolor;
	}
private:
	uint32 color;
};

extern const RGBAColor RGBA_BLACK;
extern const RGBAColor RGBA_WHITE;
extern const RGBAColor RGBA_GREY;
extern const RGBAColor RGBA_RED;
extern const RGBAColor RGBA_GREEN;
extern const RGBAColor RGBA_BLUE;
extern const RGBAColor RGBA_YELLOW;
extern const RGBAColor RGBA_MAGENTA;
extern const RGBAColor RGBA_CYAN;
extern const RGBAColor RGBA_TRANSPARENT;

struct Rect;
//-----------------------------------------------------
struct Point
{
	Point() :x(0),y(0){}
	Point( const Point &p ) { *this = p; }
	Point( int x, int y ) :x(x),y(y){}

	Point& operator = ( const Point& p )
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	Point& operator += ( const Point& p )
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	Point& operator -= ( const Point& p )
	{
		x -= p.x;
		y -= p.y;
		return *this;
	}

	Point operator + ( const Point& p ) 
	{
		Point ret(*this);
		ret += p;
		return ret;
	}

	Point operator - ( const Point& p ) 
	{
		Point ret(*this);
		ret -= p;
		return ret;
	}

	bool operator == ( const Point& p ) const
	{
		return ( (x == p.x) && (y == p.y) );
	}

	bool operator != ( const Point& p ) const
	{
		return !( (*this) == p );
	}
	

	void offset( int xoffset, int yoffset )
	{
		x += xoffset;
		y += yoffset;
	}
	
	bool isInside ( const Rect& rect ) const;

	int x;
	int y;

};

//-----------------------------------------------------
struct Size
{
	Size() : width(0), height(0) {}
	Size( const Size& s) { *this = s; }
	Size( int w, int h ) : width(w), height(h){}
		
	Size & operator=( const Size &s )
	{
		width = s.width;
		height = s.height;
		return *this;
	}

	bool operator == ( const Size& s ) const
	{
		return ( (width == s.width) && (height == s.height) );
	}

	bool operator != ( const Size& s ) const
	{
		return !( (*this) == s );
	}

	int width;
	int height;
};

//-----------------------------------------------------
struct Rect
{
	Rect() : top(0), bottom(0), left(0), right(0){}
	Rect( const Rect &r ) { *this = r; }
	Rect( int l, int r, int t, int b ) : top(t), bottom(b), left(l), right(r) {}
	Rect( Point p, Size s ) : top( p.y ), bottom( p.y + s.height ), left(p.x), right( p.x + s.width ) {}

	Rect & operator=( const Rect &r ) 
	{
		top    = r.top;
		bottom = r.bottom;
		left   = r.left;
		right  = r.right;

		return *this;
	}

	bool operator==( const Rect &r ) const
	{
		return ( (top == r.top) && (bottom == r.bottom) && (left == r.left) && (right == r.right) );
	}

	bool operator!=( const Rect &r ) const
	{
		return !( (*this) == r );
	}

	inline int x()	const { return left; }
	inline int y()	const { return top; }
	inline int width()	const { return right-left; }
	inline int height() const { return bottom-top; }

	//矩形同士の重なる部分を新たな矩形として返す
	Rect intersection( const Rect& r )
	{
		Rect ret;
		ret.top    = ( top >= r.top ) ? top : r.top;
		ret.bottom = ( bottom <= r.bottom ) ? bottom : r.bottom;
		ret.left   = ( left >= r.left ) ? left : r.left;
		ret.right  = ( right <= r.right ) ? right : r.right;
		return ret;
	}

	bool empty() const
	{
		if ( ( right <= left ) || ( bottom <= top ) ) {
			return true;
		}
		return false;
	}

	//この矩形が渡された矩形の内側(もしくは完全に重なる)ならtrue
	bool isInside( const Rect& r ) const 
	{
		return ( ( top >= r.top ) && ( bottom <= r.bottom ) && ( left >= r.left ) && ( right <= r.right ) );
	}

	int top;
	int bottom;
	int left;
	int right;
};

//-------------------------------------------------------

inline bool Point::isInside ( const Rect& rect ) const
{ 
	return (x >= rect.left) && (x <= rect.right) && (y >= rect.top) && (y <= rect.bottom); 
} 


} // namespace sgl
} // namespace sign

#endif //__SIGN_SGL_PRIMITIVES_H__

