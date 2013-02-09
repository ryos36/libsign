/* Copyright (C) 2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_POINTER_H__
#define __SIGN_SCHEME_EX_POINTER_H__

#include <stdint.h>
#include "pair_cell.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class ExPointer {
protected:
	ExPointer(); 

public:
	union {
		struct {
			ExAtomType tag;
			pair_cell *ex_pointer;
		} pointer_header;
		pair_cell _pair_cell;
	} header;

	ExAtomType get_tag() {
		return header.pointer_header.tag;
	}

	pair_cell *get_ex_pointer() {
		return header.pointer_header.ex_pointer;
	}

	explicit operator pair_cell *() {
		return get_ex_pointer();
	}
public:
	static ExPointer* alloc_ExPointer(PairCellAllocator &allocator, ExAtomType type, void *ex_pointer)
	{
		return reinterpret_cast<ExPointer *>(allocator.alloc(type, ex_pointer));
	}
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_POINTER_H__ */
