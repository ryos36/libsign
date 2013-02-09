#include "pair_cell.h"

namespace sign {
namespace scheme {

template <class X, int S>
class hand_dynamic_cast {
public:
	X func(pair_cell::cxr cxr) {
		if (!is_pointer(cxr)) {
			return 0;
		}
		if (is_pair_cell_pointer(cxr)) {
			return 0;
		}
		SIGN_ASSERT(is_ex_atom_entity_pointer(cxr));
		pair_cell *p;
		p = static_cast<pair_cell *>(cxr);
		if ( p->car.value != S ) {
			return 0;
		}
		return reinterpret_cast<X>(p);
	}
};


}}
