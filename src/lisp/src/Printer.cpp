/* Copyright (C) 2013 Sinby Corporation. All Rights Reserved. */

#include "sign/sign_assert.h"
#include <stdexcept>
#include <ostream>
#include "exception.h"
#include "pair_cell.h"
#include "ExSymbol.h"
#include "ExString.h"
#include "ExChar.h"
#include "ExExNum.h"
#include "ExBigNum.h"
#include "Printer.h"
#include "SReader.h"
#include <ctype.h>

namespace sign {
namespace scheme {

Printer::Printer(std::ostream &an_out, bool an_expand_quote) :
	out(an_out), expand_quote(an_expand_quote)
{
}

//----------------------------------------------------------------
void
Printer::print(pair_cell::cxr cxr)
{
	print_one(cxr);
}

//----------------------------------------------------------------
void
Printer::print_one(pair_cell::cxr cxr)
{
	if (is_atom(cxr)) {
		print_atom(cxr);
	} else {
		pair_cell *p;
		p = static_cast<pair_cell *>(cxr);
		if ( is_illegal_tag(p)) {
			out << "(";
			print_special_list(p);
		} else {
			print_list(p);
		}
	}
}

//----------------------------------------------------------------
void
Printer::print_atom(pair_cell::cxr cxr)
{
	if (is_nil(cxr)) {
		out << "NIL";
		return;
	}
	if (is_short_num(cxr)) {
		long int v;
		v = static_cast<long int>(cxr);
		out << v;
		return;
	}

	SIGN_ASSERT(is_ex_atom_entity_pointer(cxr));
	pair_cell *p;
	p = static_cast<pair_cell *>(cxr);

	ExAtomType type;
	type = get_ex_atom_type(p);
	switch( type ) {
	case SYMBOL:
		print_symbol(static_cast<ExSymbol *>(cxr));
		break;
	case STRING:
		print_string(static_cast<ExString *>(cxr));
		break;
	case CHAR:
		print_char(static_cast<ExChar *>(cxr));
		break;
	case EX_NUM:
		print_ex_num(static_cast<ExExNum *>(cxr));
		break;
	case BIG_NUM:
		print_big_num(static_cast<ExBigNum *>(cxr));
		break;
	case TRUE:
		out << "T";
		break;
	case SPECIAL_OPERATOR:
	case SYSTEM_FUNCTION:
	case FUNCTION:
	case COMPILED_FUNCTION:
	case MACRO:
		print_other(type, p);
		break;
	default:
		throw fatal_error("ExAtom type error");
	}
}

//----------------------------------------------------------------
void 
Printer::print_symbol(ExSymbol *symbol)
{
	out << symbol->get_c_str();
}

//----------------------------------------------------------------
void 
Printer::print_string(ExString *str)
{
	out << str->get_c_str();
}

//----------------------------------------------------------------
void 
Printer::print_char(ExChar *ex_c)
{
	char c;
	c = static_cast<char>(ex_c->get_value());
	if (isprint(c)) {
		out << "#\\" << c;
	} else {
		// TODO
		out << "#\\<" << c << ">";
	}
}

//----------------------------------------------------------------
void
Printer::print_ex_num(ExExNum *ex_num)
{
	long int value;
	value = ex_num->get_value();
	out << value;
}

//----------------------------------------------------------------
void
Printer::print_big_num(ExBigNum *big_num)
{
	// ToDo
	out << "<big num>";
}

//----------------------------------------------------------------
void
Printer::print_list(pair_cell *p)
{
	if ( !expand_quote && is_ex_atom_entity_pointer(p->car) ) {
		ExSymbol *symbol;
		symbol = static_cast<ExSymbol *>(p->car);
		const char *quote;
		if ( symbol ) {
			if ( *symbol == "QUOTE" ) {
				quote = "'";
			} else if ( *symbol == "QUASIQUOTE" ) {
				quote = "`";
			} else if ( *symbol == "UNQUOTE" ) {
				quote = ",";
			} else if ( *symbol == "UNQUOTE_SPLICING" ) {
				quote = ",@";
			} else {
				quote = 0;
			}
		} else {
			quote = 0;
		}
		if ( quote ) {
			out << quote;
			SIGN_ASSERT(p->cdr.ptr);
			SIGN_ASSERT((p->cdr.ptr->cdr.ptr == 0) || is_illegal_tag(p->cdr.ptr));
			print_one(p->cdr.ptr->car);
			return;
		}
	}
	out << "(";
	print_list_cont(p);
}

//----------------------------------------------------------------
void
Printer::print_list_cont(pair_cell *p)
{
	print_one(p->car);
	if (is_nil(p->cdr)) {
		out << ")";
		return;
	}
	out << " ";
	if (is_pointer(p->cdr)) {
		pair_cell *pp;
		pp = static_cast<pair_cell *>(p->cdr);

		if (is_illegal_tag(pp)) {
			print_special_list(pp);
			return;
		}
		if (is_pair_cell_pointer(p->cdr)) {
			print_list_cont(pp);
			return;
		}
	}
	out << ". ";
	print_one(p->cdr);
	out << ")";
}

//----------------------------------------------------------------
void
Printer::print_other(ExAtomType type, pair_cell *p)
{
	std::string str;

	switch(type) {
	case SPECIAL_OPERATOR:
		str = "#<SPECIAL_OPERATOR>";
		break;
	case SYSTEM_FUNCTION:
		str = "#<SYSTEM_FUNCTION>";
		break;
	case FUNCTION:
		str = "#<FUNCTION>";
		break;
	case COMPILED_FUNCTION:
		str = "#<COMPILED_FUNCTION>";
		break;
	case MACRO:
		str = "#<MACRO>";
		break;
	default:
		throw fatal_error("ExAtom type error");
	}
	out << str;

	switch(type) {
	case FUNCTION:
	case MACRO:
		print_one(p->cdr);
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------
void
Printer::print_special_list(pair_cell *p)
{
	SIGN_ASSERT((p->cdr.value & TAG_MASK) == ILLEGAL_TAG);
	switch( p->cdr.value ) {
	case reader::SReader::TARGET_STATUS_CAR_FREE:
		out << "[_] <status_car_free>)";
		break;
	case reader::SReader::TARGET_STATUS_CAR_OCCUPIED:
		print_one(p->car);
		out << " status_car_ocuupied>)";
		break;
	case reader::SReader::TARGET_STATUS_CAR_OCCUPIED_DOT_CDR:
		print_one(p->car);
		out << " . status_car_ocuupied_doc_cdr>)";
		break;
	case reader::SReader::TARGET_STATUS_BOTH_ARE_NULL:
		out << "[_] <status_both_are_null>)";
		break;
	}
}


} // scheme 
} // sign
