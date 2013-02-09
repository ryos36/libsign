/* Copyright (C) 2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_PRINTER_H__
#define __SIGN_SCHEME_PRINTER_H__

#include <ostream>
#include "pair_cell.h"

namespace sign {
namespace scheme {
class ExSymbol;
class ExString;
class ExChar;
class ExExNum;
class ExBigNum;
class Scheme;

class Printer {
public:
friend class Scheme;
	Printer(std::ostream &an_out, bool an_expand_quote = false);
	virtual ~Printer() {};

	virtual void print(pair_cell::cxr cxr);
	virtual void println(pair_cell::cxr cxr) {
		print(cxr);
		out << std::endl;
	}
	virtual void print(const std::string msg) {
		out << msg;
	}
	virtual void println(const std::string msg) {
		out << msg << std::endl;
	}
private:
	std::ostream &out;
	bool expand_quote;

	virtual void print_one(pair_cell::cxr cxr);
	virtual void print_atom(pair_cell::cxr cxr);
	virtual void print_list(pair_cell *p);
	virtual void print_list_cont(pair_cell *p);
	virtual void print_special_list(pair_cell *p);

	virtual void print_symbol(ExSymbol *symbol);
	virtual void print_string(ExString *str);
	virtual void print_char(ExChar *c);
	virtual void print_ex_num(ExExNum *ex_num);
	virtual void print_big_num(ExBigNum *big_num);

	virtual void print_other(ExAtomType type, pair_cell *p);
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_PRINTER_H__ */
