/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_SYMBOL_H__
#define __SIGN_SCHEME_EX_SYMBOL_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"
#include "Ex_CString.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class ExSymbol : public Ex_CString {
private:
	ExSymbol(); 

public:
	void check();
	static ExSymbol* alloc_ExSymbol(PairCellAllocator &allocator, const char *c_str);
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_SYMBOL_H__ */
