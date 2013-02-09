/* Copyright (C) 2009, 2012-2013 Sinby Corporation. All Rights Reserved. */

#include <string>
#include <ostream>
#ifdef P_DEBUG
#include <iostream>
#endif

#include "exception.h"
#include "sign/RCPointer.h"
#include "Token.h"
#include "TokenReader.h"
#include "SReader.h"
#include "Number.h"
#include "ExSymbol.h"
#include "PairCellAllocator.h"
#include "pair_cell.h"
#include <boost/lexical_cast.hpp>
#include "Printer.h"

namespace sign {
namespace scheme {
namespace reader {



SReader::SReader(PairCellAllocator &an_allocator) : allocator(an_allocator), back_pointer(0)
{
	root_cxr.ptr = 0;
}

//----------------------------------------------------------------
void
SReader::init()
{
	init_root_cxr();
}

//----------------------------------------------------------------
pair_cell::cxr 
SReader::read(TokenReader *reader)
{
	SIGN_ASSERT(!is_nil(root_cxr));
	SIGN_ASSERT(is_nil(root_cxr->car));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr.ptr->car));
	SIGN_ASSERT(is_nil(root_cxr->cdr.ptr->cdr));

	while ( true ) {
		RCPointer<Token> token;
		token = reader->get_token();

#ifdef P_DEBUG
		view(std::cout);
		std::cout << "========================" << std::endl;
		std::cout << "token type:" << token->type
			<< " str:" << token->str << std::endl;
#endif

		procedure(token);

		SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
		SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
		SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));
		if (is_nil(root_cxr->cdr->car)) {
			break;
		}
	}

	SIGN_ASSERT(is_nil(root_cxr->cdr->car));

	pair_cell::cxr rv;
	rv = root_cxr->car;

	root_cxr->car = 0;

	return rv;
}

//----------------------------------------------------------------
void
SReader::insert_target(pair_cell *new_target)
{
	pair_cell *target_pointer;
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));

	target_pointer = allocator.alloc();
	target_pointer->car = new_target;
	target_pointer->cdr = root_cxr->cdr->car;
	root_cxr->cdr->car = target_pointer;
}

//----------------------------------------------------------------
void
SReader::pop_target()
{
	pair_cell *target_pointer;
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));
	SIGN_ASSERT(!is_nil(root_cxr->cdr->car));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr->car->cdr));
	target_pointer = static_cast<pair_cell *>(root_cxr->cdr->car);
	root_cxr->cdr->car = target_pointer->cdr;

	target_pointer->car.value = 0;
	target_pointer->cdr.value = 0;

	allocator.free_pair_cell(target_pointer);
}

//----------------------------------------------------------------
int
SReader::get_target_status(pair_cell *target)
{
	SIGN_ASSERT(target);
	return target->cdr.value;
}

//----------------------------------------------------------------
void
SReader::set_target_status(pair_cell *target, int target_status)
{
	SIGN_ASSERT(target);
	target->cdr.value = target_status;
}

//----------------------------------------------------------------
void
SReader::set_cxr(pair_cell::cxr a_cxr)
{
	SIGN_ASSERT(!is_nil(root_cxr));

	SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));

	if (is_nil(root_cxr->car)) {
		SIGN_ASSERT(is_nil(root_cxr->cdr->car));

		if (is_pair_cell_pointer(a_cxr)) {
			pair_cell *target;
			target = static_cast<pair_cell *>(a_cxr);
			if ( target ) {
				SIGN_ASSERT(is_car_free(target->cdr.value));
			}
		} else {
			SIGN_ASSERT(is_atom(a_cxr));
		}

		root_cxr->car = a_cxr;

		return;
	}

	SIGN_ASSERT(!is_nil(root_cxr->cdr->car));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr->car->car));
	pair_cell *target_pointer, *target;

	target_pointer = static_cast<pair_cell *>(root_cxr->cdr->car);
	target = static_cast<pair_cell *>(target_pointer->car);
	int target_status;
	target_status = get_target_status(target);

	switch( target_status ) {
	case TARGET_STATUS_CAR_FREE:
		target->car = a_cxr;
		set_target_status(target, TARGET_STATUS_CAR_OCCUPIED);
		break;
	case TARGET_STATUS_BOTH_ARE_NULL:
		target->car = a_cxr;
		target->cdr = 0;
		pop_target();
		break;

	case TARGET_STATUS_CAR_OCCUPIED:
	{
		pair_cell *new_pair_cell;
		new_pair_cell = allocator.alloc();
		target->cdr = static_cast<pair_cell *>(new_pair_cell);

		new_pair_cell->car = a_cxr;
		set_target_status(new_pair_cell, TARGET_STATUS_CAR_OCCUPIED);
		target_pointer->car = new_pair_cell;
		break;
	}
	case TARGET_STATUS_CAR_OCCUPIED_DOT_CDR:
	{
		pair_cell *new_pair_cell;
		new_pair_cell = allocator.alloc();
		new_pair_cell->car = 0;
		set_target_status(new_pair_cell, TARGET_STATUS_BOTH_ARE_OCCUPIED_DOT_CDR);

		target->cdr = a_cxr;
		pop_target();
		insert_target(new_pair_cell);

		break;
	}
	case TARGET_STATUS_BOTH_ARE_OCCUPIED_DOT_CDR:
		syntax_error("Detect illegal data [. a b]");
	default:
		SIGN_ASSERT(0);
		fatal_error("illegal target status. what happen?");
	}

	return;
}

//----------------------------------------------------------------
void
SReader::init_root_cxr()
{
	pair_cell *p0, *p1;
	SIGN_ASSERT(is_nil(root_cxr));

	p0 = allocator.alloc();
	p1 = allocator.alloc();
	p0->car = 0;
	p0->cdr = p1;
	p1->car = 0;
	p1->cdr = 0;

	root_cxr = p0;
}

//----------------------------------------------------------------
void
SReader::add_pair_cell()
{
	SIGN_ASSERT(!is_nil(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));

	pair_cell *new_pair_cell;

	new_pair_cell = allocator.alloc();
	new_pair_cell->car = 0;
	set_target_status(new_pair_cell, TARGET_STATUS_CAR_FREE);

	// I set back pointer to car
	// since '(nil) => to nil
	pair_cell *target_pointer, *target;
	target_pointer = static_cast<pair_cell *>(root_cxr->cdr->car);
	int target_status;
	if (target_pointer) {
		target = static_cast<pair_cell *>(target_pointer->car);
		target_status = get_target_status(target);
		switch( target_status ) {
		case TARGET_STATUS_CAR_FREE:
		case TARGET_STATUS_BOTH_ARE_NULL:
			back_pointer = &target->car;
			break;
		case TARGET_STATUS_CAR_OCCUPIED:
			back_pointer = 0;
			break;
		case TARGET_STATUS_CAR_OCCUPIED_DOT_CDR:
			back_pointer = &target->cdr;
			break;
		case TARGET_STATUS_BOTH_ARE_OCCUPIED_DOT_CDR:
			back_pointer = 0;
			break;
		default:
			throw sign::scheme::fatal_error("back pointer set error");
		}
	} else {
		target_status = 0;
		back_pointer = &root_cxr->car;
	}

	pair_cell::cxr cxr;
	cxr = new_pair_cell;
	set_cxr(cxr);

	if ( target_status == TARGET_STATUS_CAR_OCCUPIED ) {
		target_pointer = static_cast<pair_cell *>(root_cxr->cdr->car);
		target = static_cast<pair_cell *>(target_pointer->car);
		target_status = get_target_status(target);
		SIGN_ASSERT(target_status == TARGET_STATUS_CAR_OCCUPIED);
		back_pointer = &target->car;
	}

	insert_target(new_pair_cell);
}

//----------------------------------------------------------------
void
SReader::add_pair_cell_quote_on_top(RCPointer<Token> token)
{
	ExSymbol *ex_symbol;
	ex_symbol = PairCellAllocator::find_symbol(allocator, token->str.c_str());
	SIGN_ASSERT(ex_symbol);
	add_pair_cell();
	pair_cell::cxr cxr;
	cxr.ex_atom = ex_symbol;
	set_cxr(cxr);
	
	cxr = static_cast<pair_cell *>(0);
	set_cxr(cxr);
	pair_cell *target_pointer, *new_pair_cell;
	target_pointer = static_cast<pair_cell *>(root_cxr->cdr->car);
	new_pair_cell = target_pointer->car.ptr;
	set_target_status(new_pair_cell, TARGET_STATUS_BOTH_ARE_NULL);
}

//----------------------------------------------------------------
void
SReader::close_list()
{
	pair_cell *target;

	target = static_cast<pair_cell *>(root_cxr->cdr->car->car);
	int target_status;
	target_status = get_target_status(target);

	switch( target_status ) {
	case TARGET_STATUS_CAR_FREE:
	{
		SIGN_ASSERT(is_pair_cell_pointer(target->car));
		SIGN_ASSERT(is_nil(target->car));
		if (back_pointer) {
			(*back_pointer).ptr = 0;
			back_pointer = 0;
		}
		target->cdr = 0;
		pop_target();
		break;
	}
	case TARGET_STATUS_CAR_OCCUPIED:
	{
		target->cdr = 0;
		pop_target();
		break;
	}
	case TARGET_STATUS_CAR_OCCUPIED_DOT_CDR:
		syntax_error("Detect no data [. )]");
		break;
	case TARGET_STATUS_BOTH_ARE_OCCUPIED_DOT_CDR:
		SIGN_ASSERT(is_nil(target->car));
		pop_target();
		break;
	default:
		SIGN_ASSERT(0);
		fatal_error("illegal target status. what happen?");
	}
}

//----------------------------------------------------------------
void
SReader::detect_dot()
{
	pair_cell *target;

	target = static_cast<pair_cell *>(root_cxr->cdr->car->car);
	int target_status;
	target_status = get_target_status(target);

	switch( target_status ) {
	case TARGET_STATUS_CAR_FREE:
		syntax_error("Detect no data [( .]");
		break;
	case TARGET_STATUS_CAR_OCCUPIED:
		set_target_status(target, TARGET_STATUS_CAR_OCCUPIED_DOT_CDR);
		break;
	case TARGET_STATUS_CAR_OCCUPIED_DOT_CDR:
		syntax_error("Detect no data [. .]");
		break;
	case TARGET_STATUS_BOTH_ARE_NULL:
		syntax_error("Detect no data maybe:[(' .]");
		break;
	default:
		SIGN_ASSERT(0);
		fatal_error("illegal target status. what happen?");
	}
}

//----------------------------------------------------------------
void
SReader::procedure(RCPointer<Token> token)
{
	pair_cell::cxr cxr;

	if ( token->type != Token::RIGHT_PAREN ) {
		back_pointer = 0;
	}

	switch (token->type) {
        case Token::NUMBER:
        case Token::SYMBOL:
        case Token::STRING:
        case Token::TRUE:
        case Token::CHAR:
                cxr = make_atom(token);
		set_cxr(cxr);
		break;
        case Token::FALSE:
		cxr.ptr = 0; // nil!!
		set_cxr(cxr);
		break;
        case Token::QUOTE:
        case Token::QUASIQUOTE:
        case Token::UNQUOTE:
        case Token::UNQUOTE_SPLICING:
                add_pair_cell_quote_on_top(token);
		break;
        case Token::LEFT_PAREN:
                add_pair_cell();
		break;

	case Token::RIGHT_PAREN:
		close_list();
		back_pointer = 0;
		break;

	case Token::DOT:
		detect_dot();
		break;

	case Token::TERMINATE:
		free_almost_pair_cell();
		syntax_error("TERMINATE");
		break;

	case Token::END_OF_READING:
		free_almost_pair_cell();
		throw eor_exception();
		break;

        case Token::SPACE:
        case Token::NEWLINE:
        case Token::NOP:
		// ignore
		break;
        default:
                fatal_error("init_procedure", 0);
		break;
	}
}
//----------------------------------------------------------------
pair_cell::cxr
SReader:: make_atom(RCPointer<Token> token)
{
	pair_cell::cxr rv;

	switch (token->type) {
        case Token::NUMBER:
	{
		RCPointer<Number> num = Number::new_number(allocator, token->str);
		rv = static_cast<pair_cell::cxr>(*num);
		break;
	}
        case Token::SYMBOL:
	{
		ExSymbol *ex_symbol;
		ex_symbol = PairCellAllocator::register_symbol(allocator, token->str.c_str());
		rv.ex_atom = ex_symbol;
		break;
	}
        case Token::STRING:
	{
		rv.ex_atom = allocator.alloc(STRING, token->str.c_str(), token->str.length() + 1);
		break;
	}
        case Token::TRUE:
	{
		rv.ex_atom = allocator.alloc(TRUE, 0);
		break;
	}
        case Token::FALSE:
	{
		rv.ptr = 0;
		break;
	}
        case Token::CHAR:
	{
		char c;
		c = token->str.c_str()[0];
		rv.ex_atom = allocator.alloc(CHAR, reinterpret_cast<void *>(c));
		break;
	}
	default:
		SIGN_ASSERT(0);
	}

	return rv;
}

//----------------------------------------------------------------
void
SReader::free_almost_pair_cell()
{
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr));
	SIGN_ASSERT(is_pair_cell_pointer(root_cxr->cdr));
	SIGN_ASSERT(is_nil(root_cxr->cdr->cdr));

	pair_cell *p, *fp, *ip;
	fp = static_cast<pair_cell *>(root_cxr);
	fp->car = 0;
	p = static_cast<pair_cell *>(fp->cdr);
	ip = static_cast<pair_cell *>(p->car);
	p->car = 0;
	SIGN_ASSERT(is_nil(p->cdr));

	while ( ip ) {
		p = static_cast<pair_cell *>(ip->cdr);

		allocator.free_pair_cell(ip);
		ip = p;
	};
}

//----------------------------------------------------------------
void
SReader::fatal_error(const char *c_msg, RCPointer<Token> token) throw(std::runtime_error&)
{
	std::string msg(c_msg);
	if ( token ) {
		msg += "Error: Token type:";
		msg += boost::lexical_cast<std::string>(token->type);
		msg += " ";
		msg += token->str;
	}

	throw sign::scheme::fatal_error(msg);
}

//----------------------------------------------------------------
void
SReader::syntax_error(const char *c_msg, RCPointer<Token> token) throw(std::runtime_error&)
{
	std::string msg(c_msg);
	if ( token ) {
		msg += "Error: Token type:";
		msg += boost::lexical_cast<std::string>(token->type);
		msg += " ";
		msg += token->str;
	}

	throw sign::scheme::syntax_error(msg);
}

} // reader
} // scheme 
} // sign

//----------------------------------------------------------------
//----------------------------------------------------------------
#include <iostream>

namespace sign {
namespace scheme {
namespace reader {

const std::string SReader::default_separate_line("--------------------------------");

//----------------------------------------------------------------
void
SReader::view(std::ostream &out)
{
	view(out, default_separate_line);
}

//----------------------------------------------------------------

void
SReader::view(std::ostream &out, const std::string &separate_line)
{
	out << separate_line << std::endl;

	if (is_nil(root_cxr)) {
		out << "root is nil" << std::endl;
		return;
	}
	/* constracting */
	pair_cell *top_pair;
	top_pair = root_cxr.ptr;
	
	out << "root car(constracting):" << top_pair->car.ptr << std::endl;
	pair_cell::cxr constracting;
	constracting = top_pair->car;
	Printer printer(out, true);
	printer.println(constracting);

	/* target */
	pair_cell *target;
	target = top_pair->cdr.ptr;
	out << "root cdr(target):" << target << std::endl;
	printer.println(top_pair->cdr);
	if ( !target ) {
		out << "ERROR!! why target is null?!" << std::endl;
		return;
	}
	pair_cell *p;
	p = target;
	while ( p ) {
		std::cout << "  " << p->car.ptr << ":" << p->cdr.ptr << std::endl;
		p = p->cdr.ptr;
	}

	/* else target */
	pair_cell *target_else;
	target_else = target->cdr.ptr;
	out << "target_else" << (target_else) << std::endl;
}


} // reader
} // scheme 
} // sign
