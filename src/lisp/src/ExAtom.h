/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_ATOM_H__
#define __SIGN_SCHEME_EX_ATOM_H__

#include <stdint.h>
#include "sign/sign_assert.h"

namespace sign {
namespace scheme {

class ExAtom {
	virtual void check() = 0;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_ATOM_H__ */
