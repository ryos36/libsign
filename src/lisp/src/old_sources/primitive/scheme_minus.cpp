#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ForEach.h"

#include "sign/scheme++/Cons.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace sign::scheme;

//------------------------------------------------------------------------
namespace {
void
do_minus(Union uarg, int &rv)
{
	if (!uarg.isAtom()) {
		throw std::runtime_error("arg must be atom.");
	}
	if (!uarg.isNumber()) {
		throw std::runtime_error("arg must be num.");
	}

	rv -= static_cast<int>(uarg);
	std::cout << rv << std::endl;
}
} // namespace

namespace sign {
namespace scheme {
namespace primitive {
Union scheme_minus(ExAtom *env, Cell *args)
{
	int r;
	Cons cons(args);
	r = 0;
	std::for_each(cons.begin(), cons.end(), boost::bind(&do_minus, _1, r));
	return Union(r);
}

} // namespace primitive
} // namespace scheme
} // namespace sign
