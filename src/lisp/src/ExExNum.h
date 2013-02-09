/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_EX_NUM_H__
#define __SIGN_SCHEME_EX_EX_NUM_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"
#include "ExValue.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

class ExExNum : public ExValue {
private:
	ExExNum(); 

public:
	static ExExNum* alloc_ExExNum(PairCellAllocator &allocator, unsigned long num) {
		return reinterpret_cast<ExExNum *>(allocator.alloc_value(EX_NUM, num));
	}
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_EX_NUM_H__ */
