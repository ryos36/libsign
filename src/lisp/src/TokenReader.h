/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_TOKEN_READER_H__
#define __SIGN_SCHEME_TOKEN_READER_H__

#include "sign/RCPointer.h"
#include "Token.h"

namespace sign {
namespace scheme {
namespace reader {

class TokenReader {
public:
	virtual ~TokenReader() {};

	virtual RCPointer<Token> get_token() = 0;
};

} // namespace reader
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_TOKEN_READER_H__ */
