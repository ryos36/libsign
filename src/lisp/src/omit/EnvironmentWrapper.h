/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_ENVIRONMENT_WRAPPER_H__
#define __SIGN_SCHEME_ENVIRONMENT_WRAPPER_H__

#include "pair_cell.h"
//#include "VolatileCxrWrapper.h"
#include "Symbol.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

class EnvironmentWrapper {
public:
	EnvironmentWrapper(pair_cell::cxr &a_env_terminal);

	void intern(PairCellAllocator *allocator, Symbol *symbol, pair_cell::cxr value);
	pair_cell::cxr getValue(const char *symbol, bool *find_flag);
	//void new_environment(pair_cell::cxr &terminal);

	bool has_next_environment();
private:
	pair_cell::cxr &env_terminal;

	pair_cell *alloc_key_value_pair(PairCellAllocator *allocator, Symbol *symbol, pair_cell::cxr value);

	pair_cell *get_env_as_ex_atom_pointer() {
	}

	pair_cell *get_mama_env_pointer() {
	}

	pair_cell *get_key_value_top_pointer();
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_ENVIRONMENT_WRAPPER_H__ */
