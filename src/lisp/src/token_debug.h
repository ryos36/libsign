/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __TOKEN_DEBUG_H__
#define __TOKEN_DEBUG_H__

#include <iostream>
#include "sign/RCPointer.h"
#include "Token.h"

namespace {

const char *token_c_str[] = {
	"NOP",
	"RIGHT_PAREN",
	"LEFT_PAREN",
	"SYMBOL",
	"NUMBER",
	"STRING",
	"QUOTE",
	"TRUE",
	"FALSE",
	"DOT",
	"CHAR",
	"SPACE",
	"NEWLINE",

	"QUASIQUOTE",
	"UNQUOTE",
	"UNQUOTE_SPLICING",

	"TERMINATE",
	"END_OF_READING",
};

void show_token(std::ostream &os, sign::RCPointer<sign::scheme::reader::Token> token)
{

	os << token_c_str[token->type] << ":" << token->str;
}

} // namespace

#endif // __TOKEN_DEBUG_H__
