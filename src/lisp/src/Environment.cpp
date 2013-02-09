/* Copyright (C) 2008, 2012-2013 Sinby Corporation. All Rights Reserved. */

#include <stdexcept>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "exception.h"
#include "Environment.h"
#include "PairCellAllocator.h"
#include "ConsChecker.h"
#include "ExSymbol.h"
#include "ExRoot.h"
#include "ExAllocator.h"

namespace sign {
namespace scheme {

Environment::Environment(pair_cell *a_env, PairCellAllocator &a_allocator)
	: env(a_env), allocator(a_allocator)
{
}

//----------------------------------------------------------------
Environment::Environment(PairCellAllocator &a_allocator)
	: env(root_to_env(a_allocator.root)), allocator(a_allocator)
{
	check_allocator();
}

//----------------------------------------------------------------
void
Environment::check_allocator()
{
	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);

	env_wrapper.check();
	ExAllocator *ex_allocator;
	ex_allocator = env_wrapper.get0();

	SIGN_ASSERT(&allocator == static_cast<PairCellAllocator *>(*ex_allocator));
	allocator.check_magic();
}

//----------------------------------------------------------------
pair_cell *
Environment::allocKeyAndValue(ExSymbol *symbol, pair_cell::cxr value) 
{
	pair_cell *key_value_pair;
	pair_cell *chain;

	chain = allocator.alloc();

	key_value_pair = allocator.alloc();

	/*
	pair_cell *p;
	p = reinterpret_cast<pair_cell *>(symbol);
	key_value_pair->car = p;
	*/
	key_value_pair->car.ex_atom = symbol;
	key_value_pair->cdr = value;
	SIGN_ASSERT(is_ex_atom_entity_pointer(key_value_pair->car));

	chain->car = key_value_pair;
	chain->cdr = 0;

	return chain;
}

//----------------------------------------------------------------
void
Environment::intern(ExSymbol *symbol, pair_cell::cxr value)
{
	pair_cell *chain;


	chain = allocKeyAndValue(symbol, value);

	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);

	env_wrapper.check();

	pair_cell *key_value_pair_top;
	key_value_pair_top = env_wrapper.get2();

	pair_cell *already_defined_value;
	already_defined_value = getKeyAndValue(key_value_pair_top, symbol);
	if ( already_defined_value ) {
		throw fatal_error("double assigned in intern");
	}

	chain->cdr = key_value_pair_top;
	env_wrapper.set2(chain);
}

//----------------------------------------------------------------
pair_cell *
Environment::getKeyAndValue(pair_cell *top, const ExSymbol *symbol)
{
	/*
	for( cons_iterator iter = top; *iter != 0 ; ++iter) {
		IsSymbol symbol_atom0(iter->car);

		*symbol_atom0 == symbol
	}
	*/

	pair_cell *iter;
	iter = top;

	while ( iter != 0 ) {
		pair_cell *kv_pair;
		SIGN_ASSERT(is_pair_cell_pointer(iter->car));
		kv_pair = static_cast<pair_cell *>(iter->car);

		SIGN_ASSERT(is_ex_atom_entity_pointer(iter->car->car));
		ExSymbol *iter_symbol;
		iter_symbol = static_cast<ExSymbol *>(kv_pair->car);
		iter_symbol->check();

		if ( *iter_symbol == *symbol ) {
			SIGN_ASSERT(is_pair_cell_pointer(iter->car));
			return kv_pair;
		}
		SIGN_ASSERT(is_pair_cell_pointer(iter->cdr));
		iter = iter->cdr.ptr;
	}
	return 0;
}

//----------------------------------------------------------------
pair_cell::cxr
Environment::getValue(const ExSymbol *symbol, bool *find_flag)
{
	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);
	env_wrapper.check();

	pair_cell *top_of_key_and_value;

	top_of_key_and_value = env_wrapper.get2();

	if ( top_of_key_and_value ) {
		pair_cell *key_and_value;
		key_and_value = getKeyAndValue(top_of_key_and_value, symbol);
		if ( key_and_value ) {
			if ( find_flag ) {
				*find_flag = true;
			}
			return key_and_value->cdr;
		}
	}

	pair_cell *mama_env_pair_cell;
	mama_env_pair_cell = env_wrapper.get1();

	if ( mama_env_pair_cell == 0 ) {
		pair_cell::cxr rv;
		rv.value = 0;
		if ( find_flag ) {
			*find_flag = false;
		}
		return rv;
	}

	Environment mama_env(mama_env_pair_cell, allocator);
	return mama_env.getValue(symbol, find_flag);
}

//----------------------------------------------------------------
bool
Environment::setValue(const ExSymbol *symbol, pair_cell::cxr value)
{

	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);
	env_wrapper.check();

	pair_cell *top_of_key_and_value;

	top_of_key_and_value = env_wrapper.get2();

	if ( top_of_key_and_value ) {
		pair_cell *key_and_value;
		key_and_value = getKeyAndValue(top_of_key_and_value, symbol);
		if ( key_and_value ) {
			key_and_value->cdr = value;
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------
void
Environment::defineValue(ExSymbol *symbol, pair_cell::cxr value)
{
	bool find;
	find = setValue(symbol, value);

	if ( find ) {
		return;
	}

	intern(symbol, value);
}

//----------------------------------------------------------------
void
Environment::defun(ExSymbol *symbol, pair_cell *args_body)
{
	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);
	env_wrapper.check();

	pair_cell *top_of_fun_and_value;

	top_of_fun_and_value = env_wrapper.get3();

	if ( top_of_fun_and_value ) {
		pair_cell *key_and_value;
		key_and_value = getKeyAndValue(top_of_fun_and_value, symbol);
		if ( key_and_value ) {
			key_and_value->cdr = args_body;
			return;
		}
	}
	pair_cell *chain;
	pair_cell::cxr value;
	value.ptr = args_body;

	chain = allocKeyAndValue(symbol, value);
	chain->cdr = top_of_fun_and_value;
	env_wrapper.set3(chain);
}

//----------------------------------------------------------------
pair_cell *
Environment::getFuncDeclare(const ExSymbol *symbol)
{
	ConsMustBe4<IsExAllocator, IsPairCellPointer, IsPairCellPointer, IsPairCellPointer> env_wrapper(env);
	env_wrapper.check();

	pair_cell *top_of_func_and_value;

	top_of_func_and_value = env_wrapper.get3();

	pair_cell *key_and_value;
	if ( top_of_func_and_value ) {
		key_and_value = getKeyAndValue(top_of_func_and_value, symbol);
	} else {
		key_and_value = 0;
	}

	return key_and_value;
}

//----------------------------------------------------------------
pair_cell *
Environment::root_to_env(pair_cell *root)
{
	pair_cell *env;
		
	SIGN_ASSERT(root);
	SIGN_ASSERT(root->car.value == ROOT);
	SIGN_ASSERT(is_pair_cell_pointer(root->cdr));
	SIGN_ASSERT(is_pair_cell_pointer(root->cdr->cdr));
	SIGN_ASSERT(is_pair_cell_pointer(root->cdr->cdr->car));
	SIGN_ASSERT(is_ex_atom_entity_pointer(root->cdr->cdr->car->car));
	SIGN_ASSERT(static_cast<pair_cell *>(root->cdr->cdr->car->car)->car.value == ALLOCATOR);

	env = static_cast<pair_cell *>(root->cdr->cdr->car);

	return env;
}


} // scheme 
} // sign
