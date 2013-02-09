#include <stdexcept>
#include <ctype.h>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"

#include "sign/scheme++/Cons.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace sign::scheme;

namespace {

typedef bool (*compare_op)(char a, char b);

bool char_le_op(char a, char b) {
	return (a) <= (b);
}

bool char_lt_op(char a, char b) {
	return (a) < (b);
}

bool char_eq_op(char a, char b) {
	return (a) == (b);
}

bool char_ge_op(char a, char b) {
	return (a) >= (b);
}

bool char_gt_op(char a, char b) {
	return (a) > (b);
}

//------------------------------------------------------------------------
bool char_le_ci_op(char a, char b) {
	return toupper(a) <= toupper(b);
}

bool char_lt_ci_op(char a, char b) {
	return toupper(a) < toupper(b);
}

bool char_eq_ci_op(char a, char b) {
	return toupper(a) == toupper(b);
}

bool char_ge_ci_op(char a, char b) {
	return toupper(a) >= toupper(b);
}

bool char_gt_ci_op(char a, char b) {
	return toupper(a) > toupper(b);
}

void
do_compare(Union uarg, compare_op op, int &n, char &last_c, bool &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isChar()) {
		throw std::runtime_error("arg must be char.");
	}

	char c;
	c = uarg.getChar();
	if ( n != 0 ) {
		rv = rv && (op(last_c, c));
	}

	last_c = c;
	++n;
}
} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union char2integer(ExAtom *env, Cell *args)
{
	if (!args->cdr.isNull()) {
		throw std::runtime_error("it requires exactly 1 argument");
	}
	Union uarg;
	uarg = args->car;
	if (!uarg.isChar()) {
		throw std::runtime_error("arg must be char.");
	}

	int c;
	c = uarg.getChar();
	return Union(c);
}

//------------------------------------------------------------------------
Union char_alphabetic_p(ExAtom *env, Cell *args)
{
	if (!args->cdr.isNull()) {
		throw std::runtime_error("it requires exactly 1 argument");
	}
	Union uarg;
	uarg = args->car;
	if (!uarg.isChar()) {
		throw std::runtime_error("arg must be char.");
	}

	int c;
	c = uarg.getChar();

	return Union(( isalpha(c) )?true:false);
}

//------------------------------------------------------------------------
Union char_le(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_le_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_lt(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_lt_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_eq(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_eq_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_ge(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_ge_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_gt(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_gt_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_le_ci(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_le_ci_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_lt_ci(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_lt_ci_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_eq_ci(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_eq_ci_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_ge_ci(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_ge_ci_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_gt_ci(ExAtom *env, Cell *args)
{
	bool b = true;
	int n = 0;
	char last_c;
	Cons cons(args);

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_compare, _1, &char_gt_ci_op, boost::ref(n), boost::ref(last_c), boost::ref(b)));
	return Union(b);
}

//------------------------------------------------------------------------
Union char_downcase(ExAtom *env, Cell *args)
{
	if (!args->cdr.isNull()) {
		throw std::runtime_error("it requires exactly 1 argument");
	}
	Union uarg;
	uarg = args->car;
	if (!uarg.isChar()) {
		throw std::runtime_error("arg must be char.");
	}

	int c;
	c = uarg.getChar();

	if ( isalpha(c) ) {
		if ( c < 'a' ) {
			c += ('a' - 'A');
			return Union(ExAtom::alloc_char(c));
		} else {
			return uarg;
		}
	} else {
		return uarg;
	}
}

//------------------------------------------------------------------------
Union char_upcase(ExAtom *env, Cell *args)
{
	ArgsIs1<IsChar> checker(args);
	checker.check();
	char c;
	c = checker.get0();

	if ( isalpha(c) ) {
		if ( 'a' <= c ) {
			c -= ('a' - 'A');
			return Union(ExAtom::alloc_char(c));
		} else {
			return Union(args);
		}
	} else {
		return Union(args);
	}
}

//------------------------------------------------------------------------
Union char_lower_case_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsChar> checker(args);
	checker.check();
	char c;
	c = checker.get0();
	bool b;

	if ( isalpha(c) ) {
		b = islower(c);
	} else {
		b = false;
	}
	return Union(b);
}

//------------------------------------------------------------------------
Union char_upper_case_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsChar> checker(args);
	checker.check();
	char c;
	c = checker.get0();
	bool b;

	if ( isalpha(c) ) {
		b = isupper(c);
	} else {
		b = false;
	}
	return Union(b);
}

//------------------------------------------------------------------------
Union char_numeric_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsChar> checker(args);
	checker.check();
	char c;
	c = checker.get0();
	bool b;

	if ( isdigit(c) ) {
		b = true;
	} else {
		b = false;
	}
	return Union(b);
}

//------------------------------------------------------------------------
Union char_whitespace_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsChar> checker(args);
	checker.check();
	char c;
	c = checker.get0();
	bool b;

	if ( c == ' ' ) {
		b = true;
	} else {
		b = false;
	}
	return Union(b);
}


} // namespace primitive
} // namespace scheme
} // namespace sign
