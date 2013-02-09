/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_NUMBER_H__
#define __SIGN_SCHEME_NUMBER_H__

#include <string>
#include <ostream>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "sign/RCPointer.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class Number {
private:
	Number(pair_cell::cxr &a_cxr) : cxr(a_cxr) {
	}

	pair_cell::cxr cxr;

public:
	explicit operator pair_cell::cxr() {
		return cxr;
	}

	static RCPointer<Number> new_number(PairCellAllocator &allocator, const std::string str);

	friend std::ostream& operator<< (std::ostream& os, const Number &n)
	{
		if (!is_ex_atom_entity_pointer(n.cxr)) {
			long int minus, v;
			SIGN_ASSERT((n.cxr.value & TAG_MASK) == ATOM_SHORT_NUM);

			v = (n.cxr.value & SHORT_ABS_NUM_MASK) >> TAG_SHIFT;
			minus = n.cxr.value & MINUS_BIT;
			if ( minus ) {
				v |= MINUS_MASK;
			}
			os << "short num:" << v ;
		} else {
			switch ( n.cxr.ptr->car.value ) {
			case REAL:
				SIGN_ASSERT((n.cxr.ptr->cdr.value & TAG_MASK) == pair_cell::CONTINOUS_BLOCK);
				os << "real:" << *reinterpret_cast<double *>(n.cxr.ptr+1);
				break;
			case EX_NUM:
				os << "ex num:" << n.cxr.ptr->cdr.value ;
				break;
			default:
				os << "error:" << n.cxr.ptr->car.value;
			}

		}
		return os;
	}
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_NUMBER_H__ */
