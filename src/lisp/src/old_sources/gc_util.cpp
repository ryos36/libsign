#include "sign/scheme++/gc.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/CellAllocator.h"
#include "sign/scheme++/Debug.h"

namespace sign {
namespace scheme {
namespace gc {

	int alloc_n;
	int gc_n;

	int ex_atom_n;

	int ex_atom_type_n[(int)ExAtom::TYPE_MAX_N];

	bool is_need_gc;

	int current_max_cell_n() {
		return CellAllocator::getCellAllocator().max_cell_n();
	}
	int current_using_cell_n() {
		return CellAllocator::getCellAllocator().using_cell_n();
	}
} // namespace gc
} // namespace scheme
} // namespace sign
