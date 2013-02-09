#include <cmath>
#include <iostream>
#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Debug.h"
#include "sign/scheme++/Number.h"

#include "sign/scheme++/Cons.h"

using namespace sign::scheme;

namespace {

//------------------------------------------------------------------------
void
plus_op(Union uarg, Number &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	rv += Number(uarg);
}

//------------------------------------------------------------------------
void
minus_op(Union uarg, Number &rv, int &n)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	if ( n == 0 ) {
		rv = Number(uarg);
	} else {
		rv -= Number(uarg);
	}
	++n;
}

//------------------------------------------------------------------------
void
mul_op(Union uarg, Number &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	rv *= Number(uarg);
}
//------------------------------------------------------------------------
void
div_op(Union uarg, Number &rv, int &n)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	Number num(uarg);
	if ( n == 0 ) {
		rv = num;
	} else {
		if ( num.eqv(0) ) {
			throw std::runtime_error("arg must be non zero.");
		}
		rv /= num;
	}
	++n;
}

//------------------------------------------------------------------------
typedef bool (*compare_op)(Number &a, Number &b);

bool less_than(Number &a, Number &b) {
	return a < b;
}

bool less_equal(Number &a, Number &b) {
	return a <= b;
}

bool num_equal(Number &a, Number &b) {
	return a == b;
	return false;
}

bool greater_than(Number &a, Number &b) {
	return a > b;
}

bool greater_equal(Number &a, Number &b) {
	return a >= b;
}

void
do_compare(Union uarg, compare_op op, int &n, Number &last, bool &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	Number v(uarg);
	if ( n != 0 ) {
		rv = rv && (op(last, v));
	}

	last = v;
	++n;
}

//------------------------------------------------------------------------
void
do_pickup(Union uarg, compare_op op, Union &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	if ( rv.isNull()) {
		rv = uarg;
	} else {
		Number num0(rv);
		Number num1(uarg);
		if ( op(num0, num1) ) {
			rv = uarg;
		}
	}
}

//------------------------------------------------------------------------
} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

//------------------------------------------------------------------------
Union scheme_plus(ExAtom *env, Cell *args)
{
	Union u(static_cast<int>(0));
	Number rv(u);
	Cons cons(args);
	rv = 0;
	std::for_each(cons.begin(), cons.end(), boost::bind(&plus_op, _1, boost::ref(rv)));
	return rv;
}

//------------------------------------------------------------------------
Union scheme_minus(ExAtom *env, Cell *args)
{
	Union u(static_cast<int>(0));
	Number rv(u);
	int n;
	Cons cons(args);
	rv = 0;
	n = 0;

	std::for_each(cons.begin(), cons.end(), boost::bind(&minus_op, _1, boost::ref(rv), boost::ref(n)));
	if ( n == 0 ) {
		throw std::runtime_error("arg must be at least 1.");
	}
	if ( n == 1 ) {
		Union u0(static_cast<int>(0));
		Number mrv(u0);
		mrv -= rv;
		return mrv;
	} else {
		return rv;
	}
}
//------------------------------------------------------------------------
Union scheme_mul(ExAtom *env, Cell *args)
{
	Union u(static_cast<int>(0));
	Number rv(u);
	Cons cons(args);

	rv = 1;

	std::for_each(cons.begin(), cons.end(), boost::bind(&mul_op, _1, boost::ref(rv)));
	return rv;
}

//------------------------------------------------------------------------
Union scheme_div(ExAtom *env, Cell *args)
{
	Union u(static_cast<int>(0));
	Number rv(u);
	int n;
	Cons cons(args);
	n = 0;

	std::for_each(cons.begin(), cons.end(), boost::bind(&div_op, _1, boost::ref(rv), boost::ref(n)));
	if ( n == 0 ) {
		throw std::runtime_error("arg must be at least 1.");
	}
	if ( n == 1 ) {
		if ( rv.eqv(0)) {
			throw std::runtime_error("arg must be none-zero.");
		}
		if ( rv.eqv(1)) {
			return Union(static_cast<int>(1));
		}
		double d;

		if ( rv.isInteger()) {
			d = rv.getInteger();
		} else {
			d = rv.getDouble();
		}
		return Union(ExAtom::alloc_double(1/d));
	} else {
		return rv;
	}
}

//------------------------------------------------------------------------
Union scheme_less_than(ExAtom *env, Cell *args)
{
	bool b = true;
	int n;
	Union ul;
	Number last(ul);
	n = 0;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &less_than, boost::ref(n), boost::ref(last), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union scheme_less_equal(ExAtom *env, Cell *args)
{
	bool b = true;
	int n;
	Union ul;
	Number last(ul);
	n = 0;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &less_equal, boost::ref(n), boost::ref(last), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union scheme_num_equal(ExAtom *env, Cell *args)
{
	bool b = true;
	Union ul;
	Number last(ul);
	int n;
	n = 0;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &num_equal, boost::ref(n), boost::ref(last), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union scheme_greater_than(ExAtom *env, Cell *args)
{
	bool b = true;
	int n;
	Union ul;
	Number last(ul);
	n = 0;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &greater_than, boost::ref(n), boost::ref(last), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union scheme_greater_equal(ExAtom *env, Cell *args)
{
	bool b = true;
	int n;
	Union ul;
	Number last(ul);
	n = 0;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &greater_equal, boost::ref(n), boost::ref(last), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union scheme_abs(ExAtom *env, Cell *args)
{
	if (!args->cdr.isNull()) {
		throw std::runtime_error("it requires exactly 1 argument");
	}
	Union uarg;
	uarg = args->car;
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	if ( uarg.isInteger()) {
		int v;
		v = abs(static_cast<int>(uarg));
		return Union(v);
	} else {
		double v;
		v = fabs(uarg.getDouble());
		return Union(ExAtom::alloc_double(v));
	}
}

//------------------------------------------------------------------------
Union scheme_zero_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union u(static_cast<int>(0));
	Number arg0(u);
	arg0 = checker.get0();

	return Union(arg0.eqv(0));
}

//------------------------------------------------------------------------
Union
scheme_max(ExAtom *env, Cell *args)
{
	Union rv;
	Cons cons(args);
	std::for_each(cons.begin(), cons.end(), boost::bind(&do_pickup, _1, &less_than, boost::ref(rv)));
	return rv;
}

//------------------------------------------------------------------------
Union
scheme_min(ExAtom *env, Cell *args)
{
	Union rv;
	Cons cons(args);
	std::for_each(cons.begin(), cons.end(), boost::bind(&do_pickup, _1, &greater_than, boost::ref(rv)));
	return rv;
}

//------------------------------------------------------------------------
Union
scheme_modulo(ExAtom *env, Cell *args)
{
	ArgsIs2<IsNumber, IsNumber> checker(args);
	checker.check();

	Union u0, u1;
	Number v0(u0), v1(u1);
	v0 = checker.get0();
	v1 = checker.get1();
	if ( v1.eqv(0)) {
		throw std::runtime_error("arg1 must be not 0.");
	}
	int i0, i1;
	i0 = v0.getInteger();
	i1 = v0.getInteger();

	int rv;
	rv = abs(i0) % abs(i1);
	if ( i1 < 0 ) {
		if ( i0 < 0 ) {
			return Union(-rv);
		} else {
			return Union(i1+rv);
		}
	} else {
		if ( i0 < 0 ) {
			return Union(i1-rv);
		} else {
			return Union(rv);
		}
	}
}

//------------------------------------------------------------------------
Union
scheme_remainder(ExAtom *env, Cell *args)
{
	ArgsIs2<IsNumber, IsNumber> checker(args);
	checker.check();

	Union u0, u1;
	Number v0(u0), v1(u1);
	v0 = checker.get0();
	v1 = checker.get1();
	if ( v1.eqv(0)) {
		throw std::runtime_error("arg1 must be not 0.");
	}
	int i0, i1;
	i0 = v0.getInteger();
	i1 = v1.getInteger();

	if ( i0 < 0 ) {
		return Union(-((-i0) % i1));
	} else {
		return Union(i0 % i1);
	}
}

//------------------------------------------------------------------------
Union scheme_odd_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union u(static_cast<int>(0));
	Number arg0(u);
	arg0 = checker.get0();
	int iv;
	iv = arg0.getInteger();

	return Union((iv % 2) != 0);
}

//------------------------------------------------------------------------
Union scheme_even_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union u(static_cast<int>(0));
	Number arg0(u);
	arg0 = checker.get0();
	int iv;
	iv = arg0.getInteger();

	return Union((iv % 2) == 0);
}

//------------------------------------------------------------------------
Union scheme_negative_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union u(static_cast<int>(0));
	Number arg0(u);
	arg0 = checker.get0();
	int iv;
	iv = arg0.getInteger();

	return Union( iv < 0 );
}

//------------------------------------------------------------------------
Union scheme_positive_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union u(static_cast<int>(0));
	Number arg0(u);
	arg0 = checker.get0();
	int iv;
	iv = arg0.getInteger();

	return Union( iv > 0 );
}

} // namespace primitive
} // namespace scheme
} // namespace sign
