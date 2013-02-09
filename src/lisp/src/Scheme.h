/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_SCHEME_H__
#define __SIGN_SCHEME_SCHEME_H__

#include <string>
#include "sign/sign_assert.h"
#include "pair_cell.h"
#include "exception.h"
#include "PairCellAllocator.h"
#include "Environment.h"
#include "SReader.h"
#include "TokenReader.h"
#include "Printer.h"

namespace sign {
namespace scheme {

class ExSymbol;

class Scheme {
public:
	Scheme(int pair_cell_n = 1024);
	void init();

	void intern(const char *c_symbol, pair_cell::cxr value);

	void repl(reader::TokenReader *reader, Printer *printer);
	void echo_on(bool an_echo = true) {
		echo = an_echo;
	}

protected:
	// for test
	pair_cell::cxr read(reader::TokenReader* reader);
	pair_cell::cxr eval(pair_cell::cxr cxr) {
		return eval(root_env, cxr);
	}
private:

	PairCellAllocator allocator;
	Environment root_env;
	reader::SReader sreader;
	bool echo;

	pair_cell *debug_env;
	pair_cell::cxr NIL;

protected:
	pair_cell::cxr eval(Environment &env, pair_cell::cxr cxr);
	void prompt(Printer *printer);

	pair_cell *deep_copy(pair_cell *p, pair_cell *rv = 0, pair_cell *prev = 0);
	pair_cell::cxr append(pair_cell *p, pair_cell::cxr cxr);
	pair_cell::cxr append(pair_cell *p, pair_cell *next_p) {
		pair_cell::cxr cxr;
		cxr.ptr = next_p;
		return append(p, cxr);
	}
	inline pair_cell *find_last(pair_cell *p);

	inline bool can_self_evaluate(pair_cell::cxr cxr);
	inline bool is_symbol(pair_cell::cxr cxr);

	inline pair_cell::cxr quasiquote(Environment &env, pair_cell::cxr, unsigned int quasiquote_n = 1, unsigned int un_quote_n = 0, bool *check_is_splicing = 0);
	inline void setq_syntax_check(pair_cell *pcell);
	inline pair_cell::cxr set_variable_value(Environment &env, pair_cell *);

	inline pair_cell::cxr set_func(Environment &env, ExSymbol *fun_name, pair_cell *func_body);
	inline pair_cell *normalize_args_declare_block(ExSymbol *block_name, pair_cell *func_body);
	pair_cell::cxr eval_sequence(Environment &env, pair_cell *seqence, bool rv_last = true);
	inline pair_cell::cxr eval_cond(Environment &env, pair_cell *seqence);
	inline pair_cell::cxr eval_let(Environment &env, pair_cell *args, pair_cell *seqence);
	inline pair_cell::cxr apply(Environment &env, pair_cell *func_body, pair_cell *specified_args);
};

//----------------------------------------------------------------

inline
pair_cell*
Scheme::find_last(pair_cell *p)
{
	SIGN_ASSERT(p);
	if ( is_nil(p->cdr) ) {
		return p;
	}
	if ( is_atom(p->cdr) ) {
		return p;
	}
	SIGN_ASSERT(is_pair_cell_pointer(p->cdr));
	return find_last(static_cast<pair_cell *>(p->cdr));
}

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_SCHEME_H__ */
