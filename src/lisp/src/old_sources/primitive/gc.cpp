#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Cons.h"
#include "sign/scheme++/Debug.h"
#include "sign/scheme++/CellAllocator.h"

using namespace sign::scheme;

//------------------------------------------------------------------------

namespace  {
} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union scheme_require_gc(ExAtom *env, Cell *args)
{
	gc::is_need_gc = true;
	return Union(&ExAtom::Unspecified);
}

} // namespace primitive
} // namespace scheme
} // namespace sign
