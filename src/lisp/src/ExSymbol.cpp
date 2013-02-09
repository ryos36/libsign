/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#include <cstring>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "ExSymbol.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

void
ExSymbol::check()
{
	SIGN_ASSERT(sizeof(header) == sizeof(pair_cell));
	SIGN_ASSERT(header.c_string_header.tag == SYMBOL);
}

//----------------------------------------------------------------
ExSymbol *
ExSymbol::alloc_ExSymbol(PairCellAllocator &allocator, const char *c_str)
{
	int len;
	len = strlen(c_str) + 1;
	pair_cell *pcell;

	std::string s(c_str);
	std::transform (s.begin (), s.end (), s.begin (), toupper);

	pcell = allocator.alloc(SYMBOL, s.c_str(), len);

	return reinterpret_cast<ExSymbol *>(pcell);
}

} // namespace scheme
} // namespace sign
