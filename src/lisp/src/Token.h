#ifndef __SIGN_SCHEME_READER_TOKEN_H__
#define __SIGN_SCHEME_READER_TOKEN_H__

#include <string>
#include <algorithm>

namespace sign {
namespace scheme {
namespace reader {

class Token {
public:
	enum Type {
		NOP,
		RIGHT_PAREN,
		LEFT_PAREN,
		SYMBOL,
		NUMBER,
		STRING,
		QUOTE,
		TRUE,
		FALSE,
		DOT,
		CHAR,
		SPACE,
		NEWLINE,

		QUASIQUOTE,
		UNQUOTE,
		UNQUOTE_SPLICING,

		TERMINATE,
		END_OF_READING,
	};

	Token(Type a_type, const std::string a_str) : type(a_type), str(a_str) {
		if ( a_type == SYMBOL ) {
			std::transform(a_str.begin(), a_str.end(), str.begin(), tolower);
		}
	}

public:
	Type type;
	std::string str;

public:
	static Token Token_QUOTE;
	static Token Token_QUASIQUOTE;
	static Token Token_UNQUOTE;
	static Token Token_UNQUOTE_SPLICING;
};

} // namespace reader
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_READER_TOKEN_H__ */
