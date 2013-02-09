/* Copyright (C) 2008, 2013 Sinby Corporation. All Rights Reserved. */

#include <cstring>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "ExString.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

void
ExString::check()
{
	SIGN_ASSERT(sizeof(header) == sizeof(pair_cell));
	SIGN_ASSERT(header.c_string_header.tag == STRING);
}

//----------------------------------------------------------------
ExString *
ExString::alloc_ExString(PairCellAllocator &allocator, const char *c_str)
{
	int len;
	len = strlen(c_str) + 1;
	pair_cell *pcell;

	pcell = allocator.alloc(STRING, c_str, len);

	return reinterpret_cast<ExString *>(pcell);
}

} // namespace scheme
} // namespace sign
