#include "vx-scheme.h"
#include "sign/sgl++/sgl.h"
#include <cmath>
#include "TestDevice.h"

using namespace sign::sgl;
extern GraphicInfo *ginfo;


const double PI = 3.14159265358979;
const int CENTER_X = TestDevice::SCREEN_WIDTH/2;
const int CENTER_Y = TestDevice::SCREEN_HEIGHT/2;

class Turtle
{
public:
	Turtle() 
		: x0(CENTER_X), y0(CENTER_Y), 
		  x1(CENTER_X), y1(CENTER_Y), 
		  direction(0) {}

	void move( int n )
	{
		x1 = x0 + (double)n * sin( PI * direction / 180.0 );
		y1 = y0 - (double)n * cos( PI * direction / 180.0 );
		ginfo->drawLine( (int)x0, (int)y0, (int)x1, (int)y1, 0xFFFFFFFF );
		x0 = x1;
		y0 = y1;
	}

	void turn( int digree )
	{
		direction += digree;
	}

private:
	double x0, y0, x1, y1;
	double direction;
};

Turtle turtle;



static Cell* dot(Context* ctx, Cell* arglist) 
{

	Cell *xy = car( arglist );
	if ( xy->type () != Cell::Cons ) {
		xy = arglist;
	}
	int x = car(xy)->IntValue();
	int y = cadr(xy)->IntValue();
	//int color = caddr (arglist)->IntValue ();
  
	ginfo->setPixel( x, y, 0xFFFF0000 );

	Cell* d = ctx->make();
	ctx->gc_protect(d);

	return d;
}  


static Cell* line(Context* ctx, Cell* arglist) 
{

	Cell *xy = car( arglist );
	if ( xy->type () != Cell::Cons ) {
		xy = arglist;
	}
	int x0 = car(xy)->IntValue();
	int y0 = car(cadr(xy))->IntValue();
	int x1 = car(cadr(cadr(xy)))->IntValue();
	int y1 = car(cadr(cadr(cadr(xy))))->IntValue();
	  
	ginfo->drawLine( x0, y0, x1, y1, 0xFFFF0000 );

	Cell* d = ctx->make();
	ctx->gc_protect(d);

	return d;
}  


static Cell* move(Context* ctx, Cell* arglist) 
{

	Cell *v = car( arglist );
	if ( v->type () != Cell::Cons ) {
		v = arglist;
	}
	int n = car(v)->IntValue();
	
	turtle.move(n);

	Cell* d = ctx->make();
	ctx->gc_protect(d);

	return d;
}  


static Cell* turn(Context* ctx, Cell* arglist) 
{

	Cell *v = car( arglist );
	if ( v->type () != Cell::Cons ) {
		v = arglist;
	}
	int n = car(v)->IntValue();
	
	turtle.turn(n);
	
	Cell* d = ctx->make();
	ctx->gc_protect(d);

	return d;
}  



class SglExtension : SchemeExtension 
{
public:
	SglExtension () { 
		Register (this);
	}

	virtual void Install (Context * ctx, Cell * envt) { 
		static struct 
		{ 
			const char* name;
			subr_f subr;
		} 
		
		bindings[] = { 
			{ "dot", dot },
			{ "move", move },
			{ "turn", turn },
		};

		static const unsigned int n_bindings = sizeof(bindings)/sizeof(*bindings);
		
		for ( unsigned int ix = 0; ix < n_bindings; ++ix ) { 
			ctx->bind_subr(bindings[ix].name, bindings[ix].subr);
		}
	}
};

static SglExtension sglExtension;

