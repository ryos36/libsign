/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#include <cstring>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "ExSymbol.h"
#include "ExRoot.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

void
ExRoot::check()
{
	SIGN_ASSERT(sizeof(header) == sizeof(pair_cell));
}

//----------------------------------------------------------------
ExSymbol *
ExRoot::search(const char *c_str)
{
	pair_cell *top_of_symbol_list, *pcell;

	SIGN_ASSERT((header.root_header.symbol_list_and_env->car.value & TAG_MASK) == 0);
	top_of_symbol_list = static_cast<pair_cell *>(header.root_header.symbol_list_and_env->car);
	pcell = top_of_symbol_list;

	while( pcell ) {
		ExSymbol *ex_symbol;

		SIGN_ASSERT(is_ex_atom_entity_pointer(pcell->car));

		ex_symbol = static_cast<ExSymbol *>(pcell->car);

		if (*ex_symbol == c_str) {
			return ex_symbol;
		}

		SIGN_ASSERT(is_pair_cell_pointer(pcell->cdr));
		pcell = static_cast<pair_cell *>(pcell->cdr);
	};

	return 0;
}

//----------------------------------------------------------------
ExSymbol *
ExRoot::register_symbol(PairCellAllocator &allocator, const char *c_str)
{
	pair_cell *top_of_symbol_list, *pcell;

	SIGN_ASSERT(is_pair_cell_pointer(header.root_header.symbol_list_and_env->car));
	top_of_symbol_list = static_cast<pair_cell *>(header.root_header.symbol_list_and_env->car);

	ExSymbol *ex_symbol;
	ex_symbol = ExSymbol::alloc_ExSymbol(allocator, c_str);

	pcell = allocator.alloc();
	pcell->car = reinterpret_cast<pair_cell *>(ex_symbol);
	pcell->cdr = top_of_symbol_list;
	header.root_header.symbol_list_and_env->car = pcell;

	return ex_symbol;
}

} // namespace scheme
} // namespace sign
