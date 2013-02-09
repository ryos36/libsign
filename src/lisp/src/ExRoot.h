/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_ROOT_H__
#define __SIGN_SCHEME_EX_ROOT_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"

namespace sign {
namespace scheme {

class ExSymbol;
class PairCellAllocator;

class ExRoot {
private:
	ExRoot(); 

public:
	union {
		struct {
			unsigned long tag;
			pair_cell *symbol_list_and_env;
		} root_header;
		pair_cell _pair_cell;
	} header;

	void check();

	ExSymbol *search(const char *c_str);
	ExSymbol *register_symbol(PairCellAllocator &allocator, const char *c_str);
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_ROOT_H__ */
