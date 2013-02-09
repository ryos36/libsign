#include <stdexcept>
#include <ctype.h>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Symbol.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Cons.h"
#include "sign/scheme++/Debug.h"

#include <boost/lexical_cast.hpp>

using namespace sign::scheme;

//------------------------------------------------------------------------

namespace  {
void
do_string_append_char(Union uarg, String &str)
{
	if (!uarg.isChar()) {
		throw std::runtime_error("arg must be char.");
	}
	str += uarg.getChar();
}

void
do_string_append(Union uarg, String &str)
{
	if (!uarg.isString()) {
		throw std::runtime_error("arg must be char.");
	}
	str += *(uarg.getStringPointer());
}

} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union number2string(ExAtom *env, Cell *args)
{
	ArgsIs1<IsNumber> checker(args);
	checker.check();
	Union a0;
	Number arg0(a0);
	arg0 = checker.get0();

	String *str;
	if ( arg0.isInteger()) {
		str = new String(boost::lexical_cast<std::string>(arg0.getInteger()));
	} else {
		str = new String(boost::lexical_cast<std::string>(arg0.getDouble()));
	}
	return Union(ExAtom::alloc(ExAtom::STRING, str));
}

//------------------------------------------------------------------------
Union string_by_char(ExAtom *env, Cell *args)
{
	Cons cons(args);
	String str;

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_string_append_char, _1, boost::ref(str)));
	return Union(ExAtom::alloc(ExAtom::STRING, new String(str)));
}

//------------------------------------------------------------------------
Union string_copy(ExAtom *env, Cell *args)
{
	ArgsIs1<IsString> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();

	return Union(ExAtom::alloc(ExAtom::STRING, new String(*arg0)));
}

//------------------------------------------------------------------------
Union string_append(ExAtom *env, Cell *args)
{
	Cons cons(args);
	String str;

	std::for_each(cons.begin(), cons.end(), boost::bind(&do_string_append, _1, boost::ref(str)));
	return Union(ExAtom::alloc(ExAtom::STRING, new String(str)));
}

//------------------------------------------------------------------------
Union string_length(ExAtom *env, Cell *args)
{
	ArgsIs1<IsString> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();

	int len;
	len = arg0->size();
	return Union(len);
}

//------------------------------------------------------------------------
Union string_ref(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsInteger> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();
	int n;
	n = checker.get1();

	int len;
	len = arg0->size();
	if (( n < 0 ) || ( len <= n )) {
		throw std::runtime_error("range is not correct.");
	}

	char c;
	c = (*arg0)[n];
	
	return Union(ExAtom::alloc_char(c));
}

//------------------------------------------------------------------------
Union string_set(ExAtom *env, Cell *args)
{
	ArgsIs3<IsString, IsInteger, IsChar> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();
	int n;
	n = checker.get1();
	char c;
	c = checker.get2();

	int len;
	len = arg0->size();
	if (( n < 0 ) || ( len <= n )) {
		throw std::runtime_error("range is not correct.");
	}

	(*arg0)[n] = c;
	
	return Union(&ExAtom::Unspecified);
}

//------------------------------------------------------------------------
Union substring(ExAtom *env, Cell *args)
{
	ArgsIs3<IsString, IsInteger, IsInteger> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();
	int start_n, end_n;
	start_n = checker.get1();
	end_n = checker.get2();

	int len;
	len = arg0->size();
	if (( start_n < 0 ) || ( len <= start_n )) {
		throw std::runtime_error("range is not correct.");
	}
	if (( end_n <= 0 ) || ( len < end_n )) {
		throw std::runtime_error("range is not correct.");
	}
	if ( start_n > end_n ) {
		throw std::runtime_error("range is not correct.");
	}
	
	return Union(ExAtom::alloc(ExAtom::STRING, new String(arg0->substr(start_n, end_n - start_n))));
}

//------------------------------------------------------------------------
Union string2number(ExAtom *env, Cell *args)
{
	ArgsIs1<IsString> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();

	int v;
	v = boost::lexical_cast<int>(arg0);
	return Union(v);
}

//------------------------------------------------------------------------
Union string2symbol(ExAtom *env, Cell *args)
{
	ArgsIs1<IsString> checker(args);
	checker.check();
	String *arg0;
	arg0 = checker.get0();

	return Union(ExAtom::alloc(ExAtom::SYMBOL, new Symbol(*arg0)));
}

//------------------------------------------------------------------------
Union symbol2string(ExAtom *env, Cell *args)
{
	ArgsIs1<IsSymbol> checker(args);
	checker.check();
	Symbol *arg0;
	arg0 = checker.get0();

	return Union(ExAtom::alloc(ExAtom::STRING, new String(arg0->name)));
}

//------------------------------------------------------------------------
Union string_ci_lt_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String str0 = *checker.get0();
	String str1 = *checker.get1();

	std::transform(str0.begin(), str0.end(), str0.begin(), tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
	return Union(str0 < str1);
}

//------------------------------------------------------------------------
Union string_ci_le_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String str0 = *checker.get0();
	String str1 = *checker.get1();

	std::transform(str0.begin(), str0.end(), str0.begin(), tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
	return Union(str0 <= str1);
}

//------------------------------------------------------------------------
Union string_ci_eq_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String str0 = *checker.get0();
	String str1 = *checker.get1();

	std::transform(str0.begin(), str0.end(), str0.begin(), tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
	return Union(str0 == str1);
}

//------------------------------------------------------------------------
Union string_ci_ge_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String str0 = *checker.get0();
	String str1 = *checker.get1();

	std::transform(str0.begin(), str0.end(), str0.begin(), tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
	return Union(str0 >= str1);
}

//------------------------------------------------------------------------
Union string_ci_gt_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String str0 = *checker.get0();
	String str1 = *checker.get1();

	std::transform(str0.begin(), str0.end(), str0.begin(), tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
	return Union(str0 > str1);
}

//------------------------------------------------------------------------
Union string_lt_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String *str0 = checker.get0();
	String *str1 = checker.get1();

	return Union(*str0 < *str1);
}

//------------------------------------------------------------------------
Union string_le_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String *str0 = checker.get0();
	String *str1 = checker.get1();

	return Union(*str0 <= *str1);
}

//------------------------------------------------------------------------
Union string_eq_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String *str0 = checker.get0();
	String *str1 = checker.get1();

	return Union(*str0 <= *str1);
}

//------------------------------------------------------------------------
Union string_ge_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String *str0 = checker.get0();
	String *str1 = checker.get1();

	return Union(*str0 >= *str1);
}

//------------------------------------------------------------------------
Union string_gt_p(ExAtom *env, Cell *args)
{
	ArgsIs2<IsString, IsString> checker(args);
	checker.check();
	String *str0 = checker.get0();
	String *str1 = checker.get1();

	return Union(*str0 > *str1);
}

//------------------------------------------------------------------------

} // namespace primitive
} // namespace scheme
} // namespace sign
