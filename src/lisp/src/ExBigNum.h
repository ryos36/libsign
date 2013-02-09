/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_BIG_NUM_H__
#define __SIGN_SCHEME_EX_BIG_NUM_H__

#include <stdint.h>
#include <cstring>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "PairCellAllocator.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class ExBigNum {
private:
	ExBigNum(); 
	explicit operator const long int *() {
		// error!!
		// you can not cast, because you lost long int size;
		return 0;
	}

public:
	union {
		struct {
			ExAtomType tag;
			unsigned long length_shift4;
		} big_num_header;
		pair_cell _pair_cell;
	} header;

	void check();

	ExAtomType get_tag() {
		return header.big_num_header.tag;
	}

	const long int *get_long_int_pointer() const {
		return reinterpret_cast<const long int *>((this + 1));
	}

	unsigned long int get_long_int_size() const {
		SIGN_ASSERT(header.big_num_header.length_shift4 & 1);
		return header.big_num_header.length_shift4 >> LENGTH_SHIFT;
	}


	bool operator < (const ExBigNum &big_num) {
		int v;
		v = cmp(big_num);
		return ( v < 0 );
	}

	bool operator == (const ExBigNum &big_num) {
		int v;
		v = cmp(big_num);
		return ( v == 0 );
	}

	bool operator < (long int value) {
		int v;
		v = cmp(value);
		return ( v < 0 );
	}

	bool operator == (long int value) {
		int v;
		v = cmp(value);
		return ( v == 0 );
	}

	static ExBigNum* alloc_ExBigNum(PairCellAllocator &allocator, const long int *big_num, int len);
	
private:
	int cmp(const ExBigNum &str) {
		return 0;
	}

	int cmp(long int value) {
		return 0;
	}
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_CHAR_H__ */
