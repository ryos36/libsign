/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_CHAR_H__
#define __SIGN_SCHEME_EX_CHAR_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"
#include "ExValue.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

class ExChar : public ExValue {
private:
	ExChar(); 

public:
	static ExChar* alloc_ExChar(PairCellAllocator &allocator, char c)
	{
		return reinterpret_cast<ExChar *>(allocator.alloc_value(CHAR, static_cast<long int>(c)));
	}
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_CHAR_H__ */
