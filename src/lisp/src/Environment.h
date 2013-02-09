/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_ENVIRONMENT_H__
#define __SIGN_SCHEME_ENVIRONMENT_H__

#include "pair_cell.h"
#include "PairCellAllocator.h"
#include "ExSymbol.h"

namespace sign {
namespace scheme {

class Scheme;

class Environment {
friend class Scheme;
private:
	Environment(pair_cell *a_env, PairCellAllocator &a_allocator);

public:
	Environment(PairCellAllocator &a_allocator);

	void intern(ExSymbol *symbol, pair_cell::cxr value);
	pair_cell::cxr getValue(const ExSymbol *symbol, bool *find_flag);
	bool setValue(const ExSymbol *symbol, pair_cell::cxr value);
	void defineValue(ExSymbol *symbol, pair_cell::cxr value);

	void defun(ExSymbol *symbol, pair_cell *args_body);
	pair_cell *getFuncDeclare(const ExSymbol *symbol);

	void check_allocator();
private:
	pair_cell *env;
	PairCellAllocator &allocator;

	pair_cell *allocKeyAndValue(ExSymbol *symbol, pair_cell::cxr value);


	static pair_cell *getKeyAndValue(pair_cell *top, const ExSymbol *symbol);
	static pair_cell *root_to_env(pair_cell *root);
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_ENVIRONMENT_H__ */
