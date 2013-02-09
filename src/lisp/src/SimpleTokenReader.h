/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_SIMPLE_TOKEN_READER_H__
#define __SIGN_SCHEME_SIMPLE_TOKEN_READER_H__

#include "sign/RCPointer.h"
#include "Token.h"
#include "TokenReader.h"
#include <list>
#include <string>

namespace sign {
namespace scheme {
namespace reader {

class SimpleTokenReader : public TokenReader {
public:
	SimpleTokenReader();
	virtual ~SimpleTokenReader();

	void set_data(const char *data_c_str);

	virtual RCPointer<Token> get_token();
private:
	std::string data;
	std::list<RCPointer<Token> > tokens;

	static std::string normalize_token_str(Token::Type type, std::string &a_token_str);
};

} // namespace reader
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_SIMPLE_TOKEN_READER_H__ */
