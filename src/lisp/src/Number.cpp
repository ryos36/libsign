/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#include <string>
#include <iostream>
#include "sign/sign_assert.h"
#include "exception.h"
#include "pair_cell.h"
#include "Number.h"
#include "PairCellAllocator.h"
#include "sign/RCPointer.h"
#include <boost/lexical_cast.hpp>

namespace sign {
namespace scheme {

RCPointer<Number>
Number::new_number(PairCellAllocator &allocator, const std::string str)
{
	pair_cell::cxr rv;

	try {
		long int v, over_flow_v;
		bool over_flow_f;
		v = boost::lexical_cast<long int>(str);
		over_flow_v = v & MINUS_MASK;

		if ( v & MINUS_BIT ) {
			// minus value
			if (( over_flow_v & MINUS_MASK ) == MINUS_MASK ) {
				over_flow_f = false;
			} else {
				over_flow_f = true;
			}
		} else if ( over_flow_v == 0 ) {
			over_flow_f = false;
		} else {
			over_flow_f = true;
		}

		if ( over_flow_f ) {
			pair_cell *p; 
			p = allocator.alloc();
			p->car.value = EX_NUM;
			p->cdr.value = v;
			
			rv = static_cast<pair_cell *>(p);
		} else {

			rv = long_int_to_cxr(v);
		}

		return RCPointer<Number>(new Number(rv));
	} catch ( boost::bad_lexical_cast ) {
		// try next pattern
	}

	try {
		double d;
		pair_cell *p; 

		d = boost::lexical_cast<double>(str);

		p = allocator.alloc(REAL, &d, sizeof(double));

		rv = static_cast<pair_cell *>(p);
		
		return RCPointer<Number>(new Number(rv));
	} catch ( boost::bad_lexical_cast &e) {
		SIGN_ASSERT(0);
		throw fatal_error("boost::bad_lexical_cast");
	}
}

} // namespace scheme
} // namespace sign
