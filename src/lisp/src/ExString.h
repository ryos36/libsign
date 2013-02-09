/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_STRING_H__
#define __SIGN_SCHEME_EX_STRING_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"
#include "Ex_CString.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class ExString : public Ex_CString {
private:
	ExString(); 

public:
	void check();
	static ExString* alloc_ExString(PairCellAllocator &allocator, const char *c_str);
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_STRING_H__ */
