/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_VOLATILE_CXR_WRAPPER_H__
#define __SIGN_SCHEME_VOLATILE_CXR_WRAPPER_H__

#include "pair_cell.h"

namespace sign {
namespace scheme {

class VolatileCxrWrapper {
public:
	VolatileCxrWrapper(pair_cell::cxr &a_cxr) : cxr(a_cxr) {}

protected:
	pair_cell::cxr &cxr;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_VOLATILE_CXR_WRAPPER_H__ */
