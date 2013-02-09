#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Debug.h"

#include "sign/scheme++/Cons.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace sign::scheme;

//------------------------------------------------------------------------

namespace  {

void
do_append(Union uarg, Cons &rv)
{
	if (!uarg.isCell()) {
		throw std::runtime_error("arg must be cell.");
	}
	if (uarg.isNull()) {
		return;		
	}
	rv.deep_copy_append(static_cast<const Cell *>(uarg));
}

void
do_reverse(Union uarg, Union &rv)
{
	Cell *cell;
	cell = Cell::alloc();
	cell->cdr = rv;
	cell->car = uarg;
	rv = Union(cell);
}

void
do_list(Union uarg, Cons &rv)
{
	rv += uarg;
}

} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union append(ExAtom *env, Cell *args)
{
	Cons rv;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_append, _1, boost::ref(rv)));
	return Union(rv.getTopCell());;
}

//------------------------------------------------------------------------
Union reverse(ExAtom *env, Cell *args)
{
	ArgsIs1<IsCell> checker(args);
	checker.check();
	Cell *arg0;
	arg0 = checker.get0();
	Union rv;
	Cons cons(arg0);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_reverse, _1, boost::ref(rv)));
	return rv;
}

//------------------------------------------------------------------------
Union cons(ExAtom *env, Cell *args)
{
	ArgsIs2<IsAny, IsAny> checker(args);
	checker.check();

	Union u0, u1;
	u0 = checker.get0();
	u1 = checker.get1();

	Cell *cell;
	cell = Cell::alloc();
	cell->car = u0;
	cell->cdr = u1;

	return Union(cell);
}

//------------------------------------------------------------------------
Union length(ExAtom *env, Cell *args)
{
	ArgsIs1<IsCell> checker(args);
	checker.check();
	Cell *arg0;
	arg0 = checker.get0();

	/*
	if ( arg0 == Cell::NIL ) {
		return Union(0);
	}
	*/

	Cons cons(arg0);

	return Union(cons.length());
}

//------------------------------------------------------------------------
Union list(ExAtom *env, Cell *args)
{
	Cons rv;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_list, _1, boost::ref(rv)));
	return Union(rv.getTopCell());;
}

//------------------------------------------------------------------------
Union set_car(ExAtom *env, Cell *args)
{
	ArgsIs2<IsCell, IsAny> checker(args);
	checker.check();
	Cell *arg0;
	arg0 = checker.get0();

	if ( arg0 == Cell::NIL ) {
		throw std::runtime_error("arg must be not '().");
	}

	Union arg1;
	arg1 = checker.get1();

	arg0->car = arg1;

	return Union(&ExAtom::Unspecified);
}

//------------------------------------------------------------------------
Union set_cdr(ExAtom *env, Cell *args)
{
	ArgsIs2<IsCell, IsAny> checker(args);
	checker.check();
	Cell *arg0;
	arg0 = checker.get0();

	if ( arg0 == Cell::NIL ) {
		throw std::runtime_error("arg must be not '().");
	}

	Union arg1;
	arg1 = checker.get1();

	arg0->cdr = arg1;

	return Union(&ExAtom::Unspecified);
}

//------------------------------------------------------------------------
Union list_ref(ExAtom *env, Cell *args)
{
	ArgsIs2<IsCell, IsInteger> checker(args);
	checker.check();
	Cell *arg0;
	arg0 = checker.get0();
	int n;
	n = checker.get1();
	if ( n < 0 ) {
		throw std::runtime_error("index out of bounds");
	}

	Cons cons(arg0);

	Cons::iterator iter(cons);
	int i;
	i = 0;
	for( iter = cons.begin(); iter != cons.end(); ++iter) {
		if ( i == n ) {
			break;
		}
		++i;
	}
	if ( iter == cons.end()) {
		throw std::runtime_error("index out of bounds");
	}
	SIGN_ASSERT(i == n);
	Union rv;
	rv = *iter;
	return rv;
}

//------------------------------------------------------------------------
Union list_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union a;
	a = checker.get0();
	do {
		if (!a.isCell()) {
			return Union(&ExAtom::False);
		}
		if ( a.isNull()) {
			return Union(&ExAtom::True);
		}
		a = a->cdr;
	} while ( true );
}


} // namespace primitive
} // namespace scheme
} // namespace sign
