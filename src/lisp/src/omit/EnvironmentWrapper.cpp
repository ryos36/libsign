/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#include <stdexcept>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "EnvironmentWrapper.h"
#include "PairCellAllocator.h"
#include "ConsChecker.h"

#define CHECK(__ptr__) 

namespace sign {
namespace scheme {

EnvironmentWrapper::EnvironmentWrapper(pair_cell::cxr &a_env_terminal) :
	env_terminal(a_env_terminal)
{
}

//----------------------------------------------------------------
pair_cell *
EnvironmentWrapper::alloc_key_value_pair(PairCellAllocator *allocator, Symbol *symbol, pair_cell::cxr value) 
{
	pair_cell *key_value_pair;
	pair_cell *chain;

	chain = allocator->alloc();
	CHECK(chain);

	key_value_pair = allocator->alloc();
	CHECK(key_value_pair);

	key_value_pair->car = allocator->alloc((ExAtomType)SYMBOL, symbol);
	key_value_pair->cdr = value;

	chain->car = key_value_pair;
	chain->cdr = 0;

	return chain;
}

//----------------------------------------------------------------
void
EnvironmentWrapper::intern(PairCellAllocator *allocator, Symbol *symbol, pair_cell::cxr value)
{
	pair_cell *chain;

	chain = alloc_key_value_pair(allocator, symbol, value);

	ConsMustBe2<IsExAtomPointer, IsPairCellPointer> env_wrapper(static_cast<pair_cell *>(env_terminal)->cdr.ptr);
	env_wrapper.check();

	pair_cell *key_value_pair_top;
	key_value_pair_top = env_wrapper.get1();

	chain->cdr = key_value_pair_top;
	env_wrapper.set1(chain);
}

//----------------------------------------------------------------
pair_cell::cxr
EnvironmentWrapper::getValue(const char *symbol, bool *find_flag)
{
	pair_cell *pcell;

	pcell = env_wrapper->getKeyValue();

	if ( env_wrapper->has_next_environment()) {
		EnvironmentWrapper next_env_wrapper(env_wrapper->next_env());
		next_env_wrapper->getValue(symbol, find_flag);
	}

	return rv;
}

//----------------------------------------------------------------
/*
void
Environment::new_environment(pair_cell::cxr &terminal)
{
	allocator.alloc_environment(terminal, static_cast<pair_cell *>(env));
}
*/

} // scheme 
} // sign
