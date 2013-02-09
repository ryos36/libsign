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

namespace debug {
int flag;
}

namespace primitive {

Union scheme_debug(ExAtom *env, Cell *args)
{
	int rv;
	rv = debug::flag;
	ArgsIs1<IsInteger> checker(args);
	try {
		checker.check();
	} catch ( const std::runtime_error &what ) {
		return Union(rv);
	}

	debug::flag = checker.get0();
	return Union(rv);
}

Union scheme_gc_stat(ExAtom *env, Cell *args)
{
	int current_max_cell_n;
	int current_using_cell_n;

	current_max_cell_n = gc::current_max_cell_n();
	current_using_cell_n = gc::current_using_cell_n();

	Cons cons;
	cons += Union(current_max_cell_n);
	cons += Union(current_using_cell_n);
	cons += Union(gc::alloc_n);
	cons += Union(gc::gc_n);
	cons += Union(gc::ex_atom_n);

	Cons cons_cell;
	for( int i = 0 ; i < ExAtom::TYPE_MAX_N ; ++i ) {
		cons_cell += Union(gc::ex_atom_type_n[i]);
	}
	cons += Union(cons_cell.getTopCell());

	return Union(cons.getTopCell());
}

} // namespace primitive
} // namespace scheme
} // namespace sign
