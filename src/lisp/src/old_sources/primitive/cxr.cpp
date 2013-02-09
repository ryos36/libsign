#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Debug.h"

#include "sign/scheme++/Cons.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace sign::scheme;

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {
//------------------------------------------------------------------------
Union cAr (ExAtom *env, Cell *args)
{
	return cxr<CAR>(env, args);
}

Union cDr (ExAtom *env, Cell *args)
{
	return cxr<CDR>(env, args);
}

} // namespace primitive
} // namespace scheme
} // namespace sign

//------------------------------------------------------------------------
CXXR_BEGIN {
	CXXR(A, A)
	CXXR(A, D)
	CXXR(D, A)
	CXXR(D, D)
} CXXR_END

CXXXR_BEGIN {
	CXXXR(A, A, A)
	CXXXR(A, A, D)
	CXXXR(A, D, A)
	CXXXR(A, D, D)
	CXXXR(D, A, A)
	CXXXR(D, A, D)
	CXXXR(D, D, A)
	CXXXR(D, D, D)
} CXXXR_END

CXXXXR_BEGIN {
	CXXXXR(A, A, A, A)
	CXXXXR(A, A, A, D)
	CXXXXR(A, A, D, A)
	CXXXXR(A, A, D, D)
	CXXXXR(A, D, A, A)
	CXXXXR(A, D, A, D)
	CXXXXR(A, D, D, A)
	CXXXXR(A, D, D, D)
	CXXXXR(D, A, A, A)
	CXXXXR(D, A, A, D)
	CXXXXR(D, A, D, A)
	CXXXXR(D, A, D, D)
	CXXXXR(D, D, A, A)
	CXXXXR(D, D, A, D)
	CXXXXR(D, D, D, A)
	CXXXXR(D, D, D, D)
} CXXXXR_END
