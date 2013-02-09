#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Cons.h"
#include "sign/scheme++/Debug.h"
#include "sign/scheme++/Display.h"

using namespace sign::scheme;

//------------------------------------------------------------------------

namespace  {
	Display display;
} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

//------------------------------------------------------------------------
Union do_display(ExAtom *env, Cell *args)
{
	ArgsIs1<IsAny> checker(args);
	checker.check();
	Union u;
	u = checker.get0();

#ifdef P_DEBUG
	std::cout << "==== debug start" << std::endl;
	print(u);
	std::cout << "==== debug e n d" 
		<< std::endl
		<< std::endl
		<< std::endl;
#endif
	display.print(u);
	
	return Union(&ExAtom::Unspecified);
}

//------------------------------------------------------------------------
Union newline(ExAtom *env, Cell *args)
{
	display.newline();
	return Union(&ExAtom::Unspecified);
}

} // namespace primitive
} // namespace scheme
} // namespace sign
