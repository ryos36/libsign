#include <iostream>
#include <cstdlib>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "sign/RCPointer.h"
#include "Token.h"
#include "SimpleTokenReader.h"
#include "SReader.h"
#include "PairCellAllocator.h"
#include "Environment.h"
#include "Number.h"
#include "ExSymbol.h"
#include <boost/lexical_cast.hpp>

#include <boost/tokenizer.hpp>

#include "token_debug.h"

using namespace sign;
using namespace sign::scheme;
using namespace sign::scheme::reader;

class TokenShow : public Token {
	friend std::ostream& operator<< (std::ostream& os, const Token &t)
	{
		os << t.type << ":" << t.str;
		return os;
	}
};

void
tokenizer(std::string str)
{
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;

	typedef boost::escaped_list_separator<char> esc_separator;
	typedef boost::tokenizer<esc_separator> tokenizer_esc;

	//char_separator sep(" \n\t", "()',", boost::drop_empty_tokens);
	char_separator sep(" \n\t", "()',");
	tokenizer tokens(str, sep);

	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
		std::cout << "<" << *tok_iter << "> ";
	}
	std::cout << std::endl;

	tokenizer_esc tokens_esc(str);

	for (tokenizer_esc::iterator tok_iter = tokens_esc.begin(); tok_iter != tokens_esc.end(); ++tok_iter) {
		std::cout << "<" << *tok_iter << "> ";
	}
	/*
	*/

	std::cout << std::endl;
}

//----------------------------------------------------------------
int
main(int argc, char **argv)
{
	SimpleTokenReader reader;
	RCPointer<Token> token;
	bool only_token_reader = false;
	const char *c_str;

	if (( argc > 1 ) && ( argv[1][0] == '-')) {
		only_token_reader = true;
		argc --;
		argv ++;
	}

	if ( argc == 1 ) {
		c_str = "( a b c )";
	} else {
		c_str = argv[1];
	}

	//tokenizer(c_str);
	reader.set_data(c_str);

	if ( only_token_reader ) {
		while ( true ) {
			token = reader.get_token();
			//std::cout << token->type << ":" << token->str << std::endl;
			show_token(std::cout, token);
			std::cout << std::endl;
			if ( token->type == Token::END_OF_READING ) {
				break;
			}
		}
		return 0;
	}

	PairCellAllocator allocator(1024);
	allocator.init();
	Environment root_env(allocator);

	PairCellAllocator::register_symbol(allocator, "QUOTE");
	PairCellAllocator::register_symbol(allocator, "QUASIQUOTE");
	PairCellAllocator::register_symbol(allocator, "UNQUOTE");
	PairCellAllocator::register_symbol(allocator, "UNQUOTE_SPLICING");

	SReader s_reader(allocator);
	s_reader.init();
	s_reader.view(std::cout, "-- Start --");
	pair_cell::cxr rv;

	rv = s_reader.read(&reader);
	s_reader.view(std::cout, "-- Finally --");

	ExSymbol *symbol_a;
	symbol_a = PairCellAllocator::find_symbol(allocator, "a");

	std::cout << "symbol_a:" << (void *)(symbol_a) << std::endl;

	return 0;
}
