/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_ALLOCATOR_H__
#define __SIGN_SCHEME_EX_ALLOCATOR_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class ExAllocator {
private:
	ExAllocator(); 

public:
	union {
		struct {
			ExAtomType tag;
			PairCellAllocator *allocator;
		} allocator_header;
		pair_cell _pair_cell;
	} header;

	void check();

	ExAtomType get_tag() {
		return header.allocator_header.tag;
	}

	explicit operator PairCellAllocator *() {
		return header.allocator_header.allocator;
	}
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_ALLOCATOR_H__ */
