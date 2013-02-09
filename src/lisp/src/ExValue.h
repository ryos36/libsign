/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_VALUE_H__
#define __SIGN_SCHEME_EX_VALUE_H__

#include <stdint.h>
#include <cstring>
#include "pair_cell.h"

namespace sign {
namespace scheme {

class ExValue {
protected:
	ExValue(); 

public:
	union {
		struct {
			ExAtomType tag;
			unsigned long value;
		} value_header;
		pair_cell _pair_cell;
	} header;

	void check();

	ExAtomType get_tag() {
		return header.value_header.tag;
	}

	unsigned long get_value() const {
		return header.value_header.value;
	}

};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_VALUE_H__ */
