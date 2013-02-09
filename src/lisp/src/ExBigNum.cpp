/* Copyright (C) 2008, 2013 Sinby Corporation. All Rights Reserved. */

#include <cstring>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "ExBigNum.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

void
ExBigNum::check()
{
	SIGN_ASSERT(sizeof(header) == sizeof(pair_cell));
	SIGN_ASSERT(header.big_num_header.tag == BIG_NUM);
}

//----------------------------------------------------------------
ExBigNum *
ExBigNum::alloc_ExBigNum(PairCellAllocator &allocator, const long int *big_num, int len)
{
	pair_cell *pcell;

	pcell = allocator.alloc(BIG_NUM, big_num, len);

	return reinterpret_cast<ExBigNum *>(pcell);
}

} // namespace scheme
} // namespace sign
