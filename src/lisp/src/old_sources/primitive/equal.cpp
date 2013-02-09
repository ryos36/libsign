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

namespace {

typedef bool ( Union::*Handler )( const Union &u ) const;

Union do_member(ExAtom *env, Cell *args, Handler handler)
{
	ArgsIs2<IsAny, IsCell> checker(args);
	checker.check();

	Union u0;
	u0 = checker.get0();

	Cell *cell;
	cell = checker.get1();
	do {
		if (cell == Cell::NIL) {
			return Union::NIL;
		}

		if ( (cell->car.*handler)(u0)) {
			return Union(cell);
		}

		if (!cell->cdr.isCell()) {
			throw std::runtime_error("args must be list.");
		}
		cell = static_cast<Cell *>(cell->cdr);

	} while (true);

	SIGN_ASSERT(0);
}

//------------------------------------------------------------------------
void
assoc_op(Union uarg, Union u0, Union &rv, Handler handler)
{
	if (!uarg.isCell()) {
		throw std::runtime_error("args must be cell.");
	}
	if (!rv.isNull()) {
		return;
	}
	if ( (uarg->car.*handler)(u0)) {
		rv = uarg;
	}
}

//------------------------------------------------------------------------
Union
do_assoc(ExAtom *env, Cell *args, Handler handler)
{
	ArgsIs2<IsAny, IsCell> checker(args);
	checker.check();

	Union u0;
	u0 = checker.get0();

	Union rv;

	Cell *cell;
	cell = checker.get1();

	Cons cons(cell);
	std::for_each(cons.begin(), cons.end(), boost::bind(&assoc_op, _1, u0, boost::ref(rv), handler));
	return rv;
}

} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union eq_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsAny, IsAny> checker(args);
	checker.check();

	Union u0, u1;
	u0 = checker.get0();
	u1 = checker.get1();
	return Union(u0.eq_p(u1));
}

//------------------------------------------------------------------------
Union eqv_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsAny, IsAny> checker(args);
	checker.check();

	Union u0, u1;
	u0 = checker.get0();
	u1 = checker.get1();
	return Union(u0.eqv_p(u1));
}

//------------------------------------------------------------------------
Union equal_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsAny, IsAny> checker(args);
	checker.check();

	Union u0, u1;
	u0 = checker.get0();
	u1 = checker.get1();

	return Union(u0.equal_p(u1));
}

//------------------------------------------------------------------------
Union memq(ExAtom *env, Cell *args)
{
	return do_member(env, args, &Union::eq_p);
}

//------------------------------------------------------------------------
Union memv(ExAtom *env, Cell *args)
{
	return do_member(env, args, &Union::eqv_p);
}

//------------------------------------------------------------------------
Union member(ExAtom *env, Cell *args)
{
	return do_member(env, args, &Union::equal_p);
}

//------------------------------------------------------------------------
Union assq(ExAtom *env, Cell *args)
{
	return do_assoc(env, args, &Union::eq_p);
}

//------------------------------------------------------------------------
Union assv(ExAtom *env, Cell *args)
{
	return do_assoc(env, args, &Union::eqv_p);
}

//------------------------------------------------------------------------
Union assoc(ExAtom *env, Cell *args)
{
	return do_assoc(env, args, &Union::equal_p);
}


} // namespace primitive
} // namespace scheme
} // namespace sign
