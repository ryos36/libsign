/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "sign/RCPointer.h"
#include "Token.h"
#include "TokenReader.h"
#include "SimpleTokenReader.h"


using namespace sign;
using namespace sign::scheme;
using namespace sign::scheme::reader;

namespace {
typedef boost::char_separator<char> char_separator;
typedef boost::tokenizer<char_separator> tokenizer;

char_separator sep(" \n\t", "()',`");
}


//----------------------------------------------------------------
SimpleTokenReader::SimpleTokenReader()
{
}

//----------------------------------------------------------------
SimpleTokenReader::~SimpleTokenReader()
{
}

//----------------------------------------------------------------
namespace {
struct equal_char_ignorecase {
	bool operator()(char x, char y) const {
		std::locale loc;
		return std::tolower(x, loc) == std::tolower(y, loc);
	}
};

bool icomp(const std::string& x, const std::string& y) {
	return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin(), equal_char_ignorecase());
}
}

//----------------------------------------------------------------
void
SimpleTokenReader::set_data(const char *data_c_str)
{
	std::list<std::string> result;

	data = data_c_str;
	boost::split(result, data, boost::is_any_of(" \t()'`,"));
	tokens.clear();
	Token *token;
	Token::Type type;

	tokenizer c_tokens(data, sep);

	BOOST_FOREACH(std::string token_str, c_tokens) {
		if ( token_str == "(" ) {
			type = Token::LEFT_PAREN;
		} else if ( token_str == ")" ) {
			type = Token::RIGHT_PAREN;
		} else {
			char c;
			c = token_str[0];

			switch( c ) {
			case '0'...'9':
				type = Token::NUMBER;
				break;
			case '"':
				type = Token::STRING;
				break;
			case '\'':
				type = Token::QUOTE;
				break;
			case '`':
				type = Token::QUASIQUOTE;
				break;
			case ',':
				type = Token::UNQUOTE;
				break;
			case '.':
				type = Token::DOT;
				break;
			default:
				if (( token_str == "t" ) || ( token_str == "T" )) {
					type = Token::TRUE;
				} else if ( icomp(token_str, "nil") ) {
					type = Token::FALSE;
				} else {
					type = Token::SYMBOL;
				}
				break;
			}
		}

		token = new Token(type, normalize_token_str(type, token_str));
		tokens.push_back(token);
	}
		
	#if 0
	// not support
	{
		UNQUOTE_SPLICING
		CHAR,
		TERMINATE,
	}
	#endif
}

//----------------------------------------------------------------
RCPointer<Token>
SimpleTokenReader::get_token()
{
	if (tokens.empty()) {
		return new Token(Token::END_OF_READING, "EOR");
	}

	RCPointer<Token> token;
	token = tokens.front();
	tokens.pop_front();

	return token;
}

//----------------------------------------------------------------
std::string
SimpleTokenReader::normalize_token_str(Token::Type type, std::string &a_token_str)
{
	switch ( type ) {
	default:
		return a_token_str;
	case Token::QUOTE:
		return "QUOTE";
	case Token::QUASIQUOTE:
		return "QUASIQUOTE";
	case Token::UNQUOTE:
		return "UNQUOTE";
	case Token::UNQUOTE_SPLICING:
		return "UNQUOTE_SPLICING";
	}
}

