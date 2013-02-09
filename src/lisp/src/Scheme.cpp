/* Copyright (C) 2008, 2012-2013 Sinby Corporation. All Rights Reserved. */

#include "exception.h"
#include "pair_cell.h"
#include "Printer.h"
#include "Scheme.h"
#include "ExRoot.h"
#include "ExSymbol.h"
#include "ExPointer.h"
#include "TokenReader.h"
#include "ConsChecker.h"

using namespace sign::scheme;
#ifdef DEBUG
#include <iostream>
namespace {
	Printer *s_printer;

	void print(const std::string msg) {
		if ( s_printer ) {
			s_printer->print(msg);
		}
	}
	void println(const std::string msg) {
		if ( s_printer ) {
			s_printer->println(msg);
		}
	}
	void print(pair_cell::cxr cxr) {
		if ( s_printer ) {
			s_printer->print(cxr);
		}
	}
	void print(pair_cell *p) {
		if ( s_printer ) {
			pair_cell::cxr cxr;
			cxr.ptr = p;
			s_printer->print(cxr);
		}
	}

	void println(pair_cell::cxr cxr) {
		if ( s_printer ) {
			s_printer->println(cxr);
		}
	}
	void println(pair_cell *p) {
		if ( s_printer ) {
			pair_cell::cxr cxr;
			cxr.ptr = p;
			s_printer->println(cxr);
		}
	}
}
#endif

Scheme::Scheme(int pair_cell_n)
	: allocator(pair_cell_n), root_env(0, allocator), sreader(allocator),
	  echo(false), debug_env(0)
{
	NIL.ptr = 0;
}

//----------------------------------------------------------------
void
Scheme::init()
{
	allocator.init();
	root_env.env = Environment::root_to_env(allocator.root);
	root_env.check_allocator();

	PairCellAllocator::register_symbol(allocator, "QUOTE");
	PairCellAllocator::register_symbol(allocator, "QUASIQUOTE");
	PairCellAllocator::register_symbol(allocator, "UNQUOTE");
	PairCellAllocator::register_symbol(allocator, "UNQUOTE_SPLICING");

	PairCellAllocator::register_symbol(allocator, "DECLARE");
	PairCellAllocator::register_symbol(allocator, "BLOCK");
	sreader.init();
}

//----------------------------------------------------------------
void
Scheme::intern(const char *c_symbol, pair_cell::cxr value)
{
	ExSymbol *symbol;
	symbol = PairCellAllocator::register_symbol(allocator, c_symbol);
	SIGN_ASSERT(symbol);
	root_env.intern(symbol, value);
}

//----------------------------------------------------------------
pair_cell *
Scheme::deep_copy(pair_cell *p, pair_cell *rv, pair_cell *prev)
{
#ifdef DEBUG
	static Printer printer(std::cout);
	s_printer = &printer;
#endif

	if ( p == 0 ) {
		return  0;
	}
	pair_cell *new_p;
	new_p = allocator.alloc();
	if ( prev ) {
		prev->cdr = new_p;
	}
	if ( rv == 0 ) {
		rv = new_p;
	}
	if ( is_atom(p->car) ) {
		new_p->car = p->car;
	} else {
		SIGN_ASSERT(is_list(p->car));
		// ToDo for embedded
		new_p->car = deep_copy(static_cast<pair_cell *>(p->car));
	}

	if ( is_atom(p->cdr) ) {
		new_p->cdr = p->cdr;
		return rv;
	} else {
		SIGN_ASSERT(is_list(p->cdr));
		// I mean using goto 
		return deep_copy(static_cast<pair_cell *>(p->cdr), rv, new_p);
	}
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::append(pair_cell *p, pair_cell::cxr cxr)
{
	if ( p == 0 ) {
		return cxr;
	}
	pair_cell *last_p;
	last_p = find_last(p);
	SIGN_ASSERT(last_p);
	if (!is_nil(last_p->cdr)) {
		// (append '(a . b) '(c d))
		throw runtime_error("proper list must not end with");
	}
	if (!is_nil(last_p->cdr)) {
		throw runtime_error("proper list must not end with");
	}
	last_p->cdr = cxr;

	pair_cell::cxr rv;
	rv.ptr = p;

	return rv;
}


//----------------------------------------------------------------
void
Scheme::repl(reader::TokenReader *reader, Printer *printer)
{
#ifdef DEBUG
	s_printer = printer;
#endif
	while( true ) {
		prompt(printer);
		pair_cell::cxr cxr;
		try {
			cxr = sreader.read(reader);
			if ( echo ) {
				printer->println(cxr);
			}
			pair_cell::cxr result;
			result = eval(root_env, cxr);
			printer->println(result);
		} catch (syntax_error &e) {
			printer->out << "Syntax Error:" << e.what() <<std::endl;
		} catch (eor_exception &e1) {
			printer->out << std::endl;
			break;
		}
	}
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::read(reader::TokenReader *reader)
{
	pair_cell::cxr cxr;
	cxr = sreader.read(reader);

	return cxr;
}


//----------------------------------------------------------------
void
Scheme::prompt(Printer *printer)
{
	printer->out << "> " << std::flush;
}

//----------------------------------------------------------------
bool
Scheme::can_self_evaluate(pair_cell::cxr cxr)
{
	if (is_nil(cxr)) {
		return true;
	}
	if (!is_atom(cxr)) {
		return false;
	}
	if (is_short_num(cxr)) {
		return true;
	}
	SIGN_ASSERT(is_ex_atom_entity_pointer(cxr));

	pair_cell *p;
	p = static_cast<pair_cell *>(cxr);
	if (p->car.value == SYMBOL) {
		return false;
	}
	return true;
}

//----------------------------------------------------------------
bool
Scheme::is_symbol(pair_cell::cxr cxr)
{
	if ( !is_ex_atom_entity_pointer(cxr) ) {
		return false;
	}

	pair_cell *p;
	p = static_cast<pair_cell *>(cxr);
	if (p->car.value == SYMBOL) {
		return true;
	}

	return false;
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::quasiquote(Environment &env, pair_cell::cxr cxr, unsigned int quasiquote_n, unsigned int un_quote_n, bool *check_is_splicing)
{
	if ( quasiquote_n <= un_quote_n ) {
		throw syntax_error("quasiquote_n");
	}

	if ( is_atom(cxr) ) {
		return cxr;
	}

	SIGN_ASSERT(is_pair_cell_pointer(cxr)) ;
	pair_cell *pcell;
	pcell = static_cast<pair_cell *>(cxr);

	bool is_splicing_body;
	bool &is_splicing = check_is_splicing?(*check_is_splicing):is_splicing_body;

	is_splicing = false;

	ExSymbol *symbol;
	symbol = static_cast<ExSymbol *>(pcell->car);

	if ( symbol ) {
		if (!is_pair_cell_pointer(pcell->cdr)) {
			throw syntax_error("CDR:cdr is not a list");
		}
		
		if (( *symbol == "UNQUOTE" ) || ( *symbol == "UNQUOTE_SPLICING" )) {
			if (!is_list(pcell->cdr)) {
				throw syntax_error("CDR:cdr is not a list");
			}
			pair_cell::cxr new_cxr;
			new_cxr = pcell->cdr.ptr->car;
			if (( quasiquote_n - un_quote_n ) == 1 ) {
				if ( check_is_splicing ) {
					*check_is_splicing = is_splicing;
				}
				return eval(env, new_cxr);
			}
			++un_quote_n;
		} else if ( *symbol == "QUASIQUOTE" ) {

			if (!is_list(pcell->cdr)) {
				throw syntax_error("CDR:cdr is not a list");
			}
			++quasiquote_n;
		}
	}

	bool local_check_is_splicing;

	pair_cell::cxr car_rv, cdr_rv;

	car_rv = quasiquote(env, pcell->car, quasiquote_n, un_quote_n, &local_check_is_splicing);
	cdr_rv = quasiquote(env, pcell->cdr, quasiquote_n, un_quote_n);

	pair_cell *new_pcell;
	new_pcell = allocator.alloc();
	new_pcell->cdr = 0;

	if ( local_check_is_splicing ) {
		SIGN_ASSERT(is_list(car_rv));
		SIGN_ASSERT(is_list(cdr_rv));
		pair_cell *top, *last;
		top = static_cast<pair_cell *>(car_rv);
		last = top;
		while ( is_nil(last->cdr)) {
			SIGN_ASSERT(is_list(last->cdr));
			last = last->cdr.ptr;
		}
		last->cdr = new_pcell;
		new_pcell->car = cdr_rv;
		return car_rv;
	} else {
		new_pcell->car = car_rv;
		new_pcell->cdr = cdr_rv;
		
		pair_cell::cxr rv;
		rv = new_pcell;
		return rv;
	}

	//throw fatal_error("Not Implement Yet");
}

//----------------------------------------------------------------
void
Scheme::setq_syntax_check(pair_cell *pcell)
{
	pair_cell *right;
	SIGN_ASSERT( pcell != 0 );

	ExSymbol *symbol;
	symbol = static_cast<ExSymbol *>(pcell->car);
	if ( symbol == 0 ) {
		//s_printer->println(pcell->car);
		// ToDo
		throw syntax_error("car is not symbol");
	}

	if (!is_list(pcell->cdr)) {
		throw syntax_error("SETQ: odd number of arguments:");
	}

	right = pcell->cdr.ptr;
	if (is_list(right->cdr)) {
		setq_syntax_check(right->cdr.ptr);
	}
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::set_variable_value(Environment &env, pair_cell *pcell)
{
	pair_cell *right;
	SIGN_ASSERT( pcell != 0 );

	ExSymbol *symbol;
	symbol = static_cast<ExSymbol *>(pcell->car);
	if ( symbol == 0 ) {
		//s_printer->println(pcell->car);
		// ToDo
		throw syntax_error("car is not symbol");
	}

	if (!is_list(pcell->cdr)) {
		throw syntax_error("SETQ: odd number of arguments:");
	}

	right = pcell->cdr.ptr;
	root_env.intern(symbol, right->car);

	if (is_list(right->cdr)) {
		return set_variable_value(env, right->cdr.ptr);
	} else {
		return right->car;
	}
}

//----------------------------------------------------------------
pair_cell *
Scheme::normalize_args_declare_block(ExSymbol *block_name, pair_cell *args_declare_block)
{
	SIGN_ASSERT(args_declare_block);

	pair_cell::cxr args, next;
	args = args_declare_block->car;
	if (!is_pair_cell_pointer(args)) {
		// detect syntax error
		// no args (defun func_name)
		throw syntax_error("missing a lambda list");
	}
	next = args_declare_block->cdr;
	if ( is_nil(next)) {
		return args_declare_block;
	}
	if ( !is_pair_cell_pointer(next) ) {
		// detect syntax error
		// like this (defun func_name () . 4)
		throw syntax_error("code contains a dotted list");
	}
	// next is atom or declare or body

	pair_cell *declare_list;
	declare_list = allocator.alloc();
	ExSymbol *symbol_declare;
	// You cannot keep symbol_declare as menber variable because of GC.
	symbol_declare = PairCellAllocator::find_symbol(allocator, "DECLARE");
	declare_list->car.ex_atom = symbol_declare;
	// '(declare )
	// TODO '(declare (system::in-defun name))

	pair_cell *next_p;
	do {

		next_p = static_cast<pair_cell *>(next);

		pair_cell::cxr next_car;
		next_car = next_p->car;

		bool has_declare;
		has_declare = false;

		if (is_list(next_car)) {
			// check (declare ....)
			// car is declare

			ExSymbol *symbol;
			symbol = static_cast<ExSymbol *>(next_car->car);
			if (symbol && (*symbol == "DECLARE")) {
				has_declare = true;
			}
		}
		if ( !has_declare ) {
			break;
		}
		// has DECLARE
		if ( declare_list == 0 ) {
			declare_list = static_cast<pair_cell *>(next_car);
		} else {
			if (!is_pair_cell_pointer(next_car->cdr)) {
				throw syntax_error("code contains a dotted list");
			}
			append(declare_list, next_car->cdr);
		}

		next = next_p->cdr;
	} while ( is_list(next) );

	if (!is_list(next) ) {
		if (!is_nil(next)) {
			// detect syntax error
			// like this (defun func_name () (declare ...)  . 4)
			throw syntax_error("code contains a dotted list");
		}
		// no-body
	}

	pair_cell *block_top, *body_top;
	body_top = static_cast<pair_cell *>(next);

	ExSymbol *symbol_block;
	// You cannot keep symbol_declare as menber variable because of GC.
	symbol_block = PairCellAllocator::find_symbol(allocator, "BLOCK");

	block_top = PairCellAllocator::alloc_set2(allocator, symbol_block, block_name);
	append(block_top, body_top);

	pair_cell *rv;
	rv = PairCellAllocator::alloc_set3(allocator, 0, declare_list, block_top);
	rv->car = args;

	return rv;
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::set_func(Environment &env, ExSymbol *func_name, pair_cell *args_declare_block)
{
	pair_cell *ex_fun;

	pair_cell *norm_func = normalize_args_declare_block(func_name, deep_copy(args_declare_block));
	ex_fun = allocator.alloc(FUNCTION, norm_func);
	SIGN_ASSERT(func_name);
	root_env.defun(func_name, ex_fun);

	pair_cell::cxr rv;
	rv = ex_fun;
	return rv;
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::eval_sequence(Environment &env, pair_cell *seqence, bool rv_last)
{
	pair_cell *seqi;
	seqi = seqence;
	bool first;
	pair_cell::cxr first_rv, rv;
	first = true;
	do {
		if (!is_nil(seqi->cdr) && is_atom(seqi->cdr)) {
			throw sign::scheme::runtime_error("dotted parameter");
		}
		rv = eval(env, seqi->car);
		if (first) {
			first_rv = rv;
			first = false;
		}
		seqi = static_cast<pair_cell *>(seqi->cdr);
	} while (seqi != 0);

	return rv_last?rv:first_rv;
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::eval_cond(Environment &env, pair_cell *cond_seqence)
{
	pair_cell *seqi;
	seqi = cond_seqence;
	pair_cell::cxr eval_val, rv;
	do {
		if (!is_nil(seqi->cdr) && is_atom(seqi->cdr)) {
			throw sign::scheme::runtime_error("dotted parameter");
		}
		if (!is_list(seqi->car)) {
			throw sign::scheme::runtime_error("clause should be a list");
		}
		pair_cell *list;
		list = static_cast<pair_cell *>(seqi->car);

		if (!is_pair_cell_pointer(list->cdr)) {
			throw sign::scheme::runtime_error("dotted parameter");
		}

		eval_val = eval(env, list->car);
		if (!is_nil(eval_val)) {
			if ( is_nil(list->cdr) ) {
				rv = eval_val;
			} else {
				pair_cell *seq;
				seq = static_cast<pair_cell *>(list->cdr);
				rv = eval_sequence(env, seq);
			}
			return rv;
		}
		seqi = static_cast<pair_cell *>(seqi->cdr);
	} while (seqi != 0);

	return NIL;
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::eval_let(Environment &env, pair_cell *args, pair_cell *seqence)
{
	pair_cell *new_env_cell;
	new_env_cell = allocator.alloc_environment(env.env);
	Environment new_env(new_env_cell, allocator);

	pair_cell *argsi;
	for( argsi = args; argsi; argsi = static_cast<pair_cell *>(argsi->cdr)) {
		pair_cell::cxr &one_arg(argsi->car);

		if (is_nil(one_arg)) {
			throw sign::scheme::runtime_error("args form nil error");
		}
		ExSymbol *symbol;
		if (is_list(one_arg)) {
			symbol = static_cast<ExSymbol *>(one_arg->car);
		} else {
			symbol = static_cast<ExSymbol *>(one_arg);
		}
		if ( !symbol ) {
			throw sign::scheme::runtime_error("args form symbol error");
		}
		pair_cell::cxr value;
		if (is_list(one_arg) && !is_nil(one_arg->cdr)) {
			// use old env!!
			if (!is_list(one_arg->cdr)) {
				throw sign::scheme::runtime_error("args form dotted error");
			}
			value = eval(env, one_arg->cdr->car);
		} else {
			value = NIL;
		}
		new_env.intern(symbol, value);
		
		if (!is_pair_cell_pointer(argsi->cdr)) {
			throw sign::scheme::runtime_error("args form error");
		}
	}

	return eval_sequence(new_env, seqence);
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::apply(Environment &env, pair_cell *func_body, pair_cell *specified_args)
{
	pair_cell *args_symbol;
	pair_cell *declare_body;
	pair_cell *body;
	args_symbol = static_cast<pair_cell *>(func_body->car);
	if (!is_list(func_body->cdr) ) {
		throw sign::scheme::runtime_error("func form declare error");
	}
	declare_body = static_cast<pair_cell *>(func_body->cdr);
	// ToDo:now declare is not used.
	if (!is_list(declare_body->cdr) ) {
		throw sign::scheme::runtime_error("func form body error");
	}
	body = static_cast<pair_cell *>(declare_body->cdr);

	pair_cell *new_env_cell;
	new_env_cell = allocator.alloc_environment(env.env);
	Environment new_env(new_env_cell, allocator);

	pair_cell *argsi, *s_argsi;
	argsi = args_symbol;
	s_argsi = specified_args;
	while(argsi && s_argsi) {
		ExSymbol *symbol;

		symbol = static_cast<ExSymbol *>(argsi->car);
		if ( !symbol ) {
			throw sign::scheme::runtime_error("args form symbol error");
		}
		pair_cell::cxr value;
		value = eval(env, s_argsi->car);
		new_env.intern(symbol, value);

		if (!is_pair_cell_pointer(argsi->cdr)) {
			throw sign::scheme::runtime_error("args form error");
		}
		if (!is_pair_cell_pointer(s_argsi->cdr)) {
			throw sign::scheme::runtime_error("specified args form error");
		}
		argsi = static_cast<pair_cell *>(argsi->cdr);
		s_argsi = static_cast<pair_cell *>(s_argsi->cdr);
	}
	if ( argsi || s_argsi ) {
		throw sign::scheme::runtime_error("args is not match");
	}

	return eval_sequence(new_env, body);
}

//----------------------------------------------------------------
pair_cell::cxr
Scheme::eval(Environment &env, pair_cell::cxr cxr)
{
	debug_env = env.env;
	pair_cell::cxr rv;

	if (can_self_evaluate(cxr)) {
		return cxr;
	}

	// can evaluate as symbol?
	ExSymbol *symbol;
	symbol = static_cast<ExSymbol *>(cxr);
	if ( symbol ) {
		bool find_flag;
		rv = env.getValue(symbol, &find_flag);
		if ( !find_flag ) {
			throw sign::scheme::runtime_error("undefined value");
		}
		return rv;
	}

	pair_cell *pcell;

	SIGN_ASSERT(is_pointer(cxr));
	if (!is_pointer(cxr)) {
		throw syntax_error("not pointer cxr");
	}

	pcell = static_cast<pair_cell *>(cxr);
	
	symbol = static_cast<ExSymbol *>(pcell->car);

	if ( symbol == 0 ) {
		// application
		print(cxr);
		throw fatal_error("Not Implement Yet");
	}

	// check quoted 
	if ( *symbol == "QUOTE" ) {
		if (!is_list(pcell->cdr)) {
			throw syntax_error("CDR:cdr is not a list");
		}
		return pcell->cdr.ptr->car;
	}

	// check quasiquote 
	if ( *symbol == "QUASIQUOTE" ) {
		if (!is_list(pcell->cdr)) {
			throw syntax_error("CDR:cdr is not a list");
		}
		return quasiquote(env, pcell->cdr.ptr->car);
	}

	// check setq
	if ( *symbol == "SETQ" ) {
		if (!is_list(pcell->cdr)) {
			throw syntax_error("CDR:cdr is not a list");
		}
		setq_syntax_check(pcell->cdr.ptr);
		return set_variable_value(env, pcell->cdr.ptr);
	}

	if ( *symbol == "BLOCK" ) {
		if (!is_list(pcell->cdr)) {
			throw syntax_error("block must be list");
		}
		pair_cell *bname_body;
		bname_body = static_cast<pair_cell *>(pcell->cdr);
		if ( static_cast<ExSymbol *>(bname_body->car) == 0 ) {
			throw syntax_error("block name is failed");
		}
		pair_cell::cxr body;
		body = bname_body->cdr;
		if (is_nil(body)) {
			return NIL;
		}
		if (is_atom(body)) {
			// (block bname . t)
			throw syntax_error("block dotted error");
		}
		pair_cell *seqence;
		seqence = static_cast<pair_cell *>(body);
		return eval_sequence(env, seqence);
	}

	if ( *symbol == "DEFUN" ) {
		ConsMustBeAtLeast2<IsExAtom<ExSymbol>, IsPairCellPointer> arg_wrapper(static_cast<pair_cell *>(pcell->cdr));

		arg_wrapper.check(); // ToDo just syntax error
		ExSymbol *func_name;
		func_name = arg_wrapper.get0();
		SIGN_ASSERT(func_name);

		//func_name = pcell->cdr.ptr->car;
		//args... = pcell->cdr.ptr->cdr.ptr;
		return set_func(env, func_name, pcell->cdr.ptr->cdr.ptr);
	}

	if ( *symbol == "IF" ) {
		/*
		ConsMustBeAtLeast2<IsPairCellPointer, IsPairCellPointer> arg_wrapper(predicate_then_else);
		arg_wrapper.check(); // ToDo just syntax error
		*/
		if (!is_list(pcell->cdr)) {
			throw sign::scheme::runtime_error("no predicate");
			// ToDo message
		}

		pair_cell *predicate_then_else;
		predicate_then_else = static_cast<pair_cell *>(pcell->cdr);
		if (!is_list(predicate_then_else->cdr)) {
			throw sign::scheme::runtime_error("no then");
			// ToDo message
		}
		pair_cell *then_else;
		then_else = static_cast<pair_cell *>(predicate_then_else->cdr);
		if ((!is_nil(then_else->cdr)) && is_atom(then_else->cdr)) {
			throw sign::scheme::runtime_error("dotted parameter");
			// ToDo message
		}

		pair_cell::cxr predicate_result;
		predicate_result = eval(env, predicate_then_else->car);
		if (!is_nil(predicate_result)) {
			return eval(env, then_else->car);
		} else {
			if (is_nil(then_else->cdr)) {
				return NIL;
			}
			pair_cell *else_clause;
			else_clause = static_cast<pair_cell *>(then_else->cdr);
			if (!is_nil(else_clause->cdr) ) {
				// ToDo
				// syntax check wo chanto
				// eval(env, predicate_then_else->car);
				// no maeni yarubeki
				throw sign::scheme::runtime_error("too many parameters");
			}

			return eval(env, else_clause->car);
		}
	}

	if ( *symbol == "PROGN" ) {
		if (is_nil(pcell->cdr)) {
			return NIL;
		}
		if (is_atom(pcell->cdr)) {
			// detect (progn . t)
			throw syntax_error("code contains a dotted list");
		}
		pair_cell *seqence;
		seqence = static_cast<pair_cell *>(pcell->cdr);
		return eval_sequence(env, seqence);
	}

	if ( *symbol == "PROG1" ) {
		if (is_nil(pcell->cdr)) {
			throw syntax_error("too few parameter");
		}
		if (is_atom(pcell->cdr)) {
			// detect (progn . t)
			throw syntax_error("code contains a dotted list");
		}
		pair_cell *seqence;
		seqence = static_cast<pair_cell *>(pcell->cdr);
		return eval_sequence(env, seqence, false);
	}

	if ( *symbol == "COND" ) {
		if (is_nil(pcell->cdr)) {
			return NIL;
		}

		if (is_atom(pcell->cdr)) {
			// detect (cond . t)
			throw syntax_error("code contains a dotted list");
		}
		pair_cell *cond_seqence;
		cond_seqence = static_cast<pair_cell *>(pcell->cdr);
		return eval_cond(env, cond_seqence);
	}

	if ( *symbol == "LET" ) {
		if (is_nil(pcell->cdr)) {
			throw syntax_error("too few parameter");
		}
		if (is_atom(pcell->cdr)) {
			// detect (let . t)
			throw syntax_error("code contains a dotted list");
		}
		pair_cell *let_args_seqence;
		let_args_seqence = static_cast<pair_cell *>(pcell->cdr);
		if (!is_pair_cell_pointer(let_args_seqence->car)) {
			// detect (let a ...)
			throw syntax_error("form error");
		}
		if (is_nil(let_args_seqence->cdr)) {
			// detect (let (...))
			return NIL;
		}
		if (is_atom(let_args_seqence->cdr)) {
			// detect (let (...) . t)
			throw syntax_error("code contains a dotted list");
		}
		pair_cell *args, *seqence;
		args = static_cast<pair_cell *>(let_args_seqence->car);
		seqence = static_cast<pair_cell *>(let_args_seqence->cdr);
		return eval_let(env, args, seqence);
	}

	// call funcs
	if (!is_pair_cell_pointer(pcell->cdr)) {
		// detect (func . a)
		throw sign::scheme::runtime_error("args form dotted error");
	}
	pair_cell *specified_args;
	specified_args = static_cast<pair_cell *>(pcell->cdr);

	pair_cell *func_declare;
	func_declare = env.getFuncDeclare(symbol);
	if ( func_declare == 0 ) {
		throw sign::scheme::runtime_error("undefined function");
	}
	ExPointer *ex_pointer;
	ex_pointer = static_cast<ExPointer *>(func_declare->cdr);
	SIGN_ASSERT(ex_pointer);

	pair_cell *func_body;
	func_body = ex_pointer->get_ex_pointer();

	if (!is_pair_cell_pointer(func_body->car)) {
		throw sign::scheme::runtime_error("illegal function is defined.");
	}

	switch(ex_pointer->get_tag()) {
	case FUNCTION:
		return apply(env, func_body, specified_args);
	case SPECIAL_OPERATOR:
	case SYSTEM_FUNCTION:
	case COMPILED_FUNCTION:
	case MACRO:
		throw sign::scheme::runtime_error("Not Implement Yet FUNC");
		break;
	default:
		SIGN_ASSERT(0);
		break;
	}

	throw sign::scheme::runtime_error("No Function");
}
