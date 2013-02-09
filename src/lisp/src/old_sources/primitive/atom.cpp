#include <cmath>
#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Cons.h"
#include "sign/scheme++/Debug.h"

using namespace sign::scheme;

//------------------------------------------------------------------------

namespace  {
} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union boolean_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isBoolean());
}

//------------------------------------------------------------------------
Union char_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isChar());
}

//------------------------------------------------------------------------
Union integer_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	if (!arg0.isNumber()) {
		return Union::False;
	}
	if (arg0.isInteger()) {
		return Union::True;
	}
	double d;
	d = arg0.getDouble();
	return Union((d - floor(d)) == 0);
}

//------------------------------------------------------------------------
Union null_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isNull());
}

//------------------------------------------------------------------------
Union number_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isNumber());
}

//------------------------------------------------------------------------
Union pair_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isPair());
}

//------------------------------------------------------------------------
Union string_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isString());
}

//------------------------------------------------------------------------
Union symbol_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isSymbol());
}

//------------------------------------------------------------------------
Union procedure_p(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	return Union(arg0.isProcedure());
}

//------------------------------------------------------------------------
Union scheme_not(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union arg0;
	arg0 = checker.get0();

	if (arg0.isFalse()) {
		return Union(&ExAtom::True);
	} else {
		return Union(&ExAtom::False);
	}
}

} // namespace primitive
} // namespace scheme
} // namespace sign
