#include <stdexcept>
#include "sign/sign_assert.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Cons.h"

using namespace sign::scheme;

//------------------------------------------------------------------------
Cons::Cons(Cell *cell) : cell(cell), last(0)
{
	if ( cell == Cell::NIL ) {
		return;
	}
	
	resetLast();
}

//------------------------------------------------------------------------
Cons::Cons(const Cons &cons) : last(0)
{
	Union copy_u;
	copy_u = Union::deep_copy(Union(cons.cell));
	cell = static_cast<Cell *>(copy_u);

	resetLast();
}

//------------------------------------------------------------------------
void
Cons::resetLast()
{
	Cell *start;
	if ( last == 0 ) {
		if ( cell == 0 ) {
			return;
		}
		start = cell;
	} else {
		start = last;
	}
	Cell *cp;
	for( cp = start ; cp->cdr != Cell::NIL ; cp = cp->cdr ) {
		SIGN_ASSERT(cp->cdr.isCell());
	}
	SIGN_ASSERT(cp->cdr.isNull());
	last = cp;
}

//------------------------------------------------------------------------
Cons &
Cons::operator +=(const Union u)
{
	Cell *c;
	c = Cell::alloc();
	SIGN_ASSERT(c->cdr.isNull());

	if ( cell == 0 ) {
		last = cell = c;
	} else {
		last->cdr = c;
		last = c;
	}
	last->car = u;

	return *this;
}

//------------------------------------------------------------------------
Cons &
Cons::operator +=(Cell *cp)
{
	if ( cell == 0 ) {
		last = cell = cp;
	} else {
		last->cdr = cp;
	}

	resetLast();

	return *this;
}

//------------------------------------------------------------------------
Cons &
Cons::operator +=(ExAtom *ex_atom)
{
	Cell *c;
	c = Cell::alloc();

	if ( cell == 0 ) {
		last = cell = c;
	} else {
		last->cdr = c;
		last = c;
	}
	last->car = ex_atom;

	return *this;
}

//------------------------------------------------------------------------
Union
Cons::operator [](int n)
{
	Cell *cp;
	cp = cell;
	for ( int i = 0 ; i < n; ++i ) {
		SIGN_ASSERT(cp->cdr.isCell());
		cp = cp->cdr;
		SIGN_ASSERT(cp != Cell::NIL);
	}
	SIGN_ASSERT(cp != Cell::NIL);
	return cp->car;
}

//------------------------------------------------------------------------
void
Cons::deep_copy_append(const Cell *cp)
{
	Union copy_u;
	copy_u = Union::deep_copy(Union(const_cast<Cell *>(cp)));
	(*this) += static_cast<Cell *>(copy_u);
}

//------------------------------------------------------------------------
int
Cons::length() const
{
	int n;
	n = 0;
	Cell *cp;
	for( cp = cell ; cp != Cell::NIL ; cp = cp->cdr ) {
		++n;
		SIGN_ASSERT(cp->cdr.isCell());
	}
	return n;
}

//------------------------------------------------------------------------
bool
Cons::isValid(int n) const
{
	Cell *cp;
	cp = cell;
	if ( cp == Cell::NIL ) {
		return false;
	}
	for ( int i = 0 ; i < n; ++i ) {
		SIGN_ASSERT(cp->cdr.isCell());
		cp = cp->cdr;
		if ( cp == Cell::NIL ) {
			return false;
		}
	}
	if ( cp == Cell::NIL ) {
		return false;
	}
	return cp->car;
}
//------------------------------------------------------------------------
Cell *
Cons::getTopCell()
{
	return cell;
}

//------------------------------------------------------------------------
Cons::iterator
Cons::begin()
{
	return Cons::iterator(*this, cell);
}

//------------------------------------------------------------------------
Cons::iterator
Cons::end()
{
	return Cons::iterator(*this, Cell::NIL);
}
