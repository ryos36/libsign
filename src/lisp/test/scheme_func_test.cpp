#include <iostream>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#define P_DEBUG
#include "PairCellAllocator.h"
#include "Environment.h"
#include "Scheme.h"
#include "SimpleTokenReader.h"
#include "Printer.h"

using namespace sign::scheme;
using namespace sign::scheme::reader;

class SchemeTest : public Scheme {
public:
	SchemeTest() : Scheme(1024) {}

	pair_cell *deep_copy(pair_cell *p) {
		return Scheme::deep_copy(p);
	}

	pair_cell::cxr append(pair_cell *p, pair_cell::cxr cxr) {
		return Scheme::append(p, cxr);
	}

	pair_cell *find_last(pair_cell *p) {
		return Scheme::find_last(p);
	}

	pair_cell::cxr read(reader::TokenReader* reader) {
		return Scheme::read(reader);
	}

	pair_cell::cxr eval(pair_cell::cxr cxr) {
		return Scheme::eval(cxr);
	}
};

//----------------------------------------------------------------
void
scheme_test(const char cmd, const char *c_str)
{
	SchemeTest st;

	st.init();
	SimpleTokenReader reader;
	Printer printer(std::cout);

	st.echo_on();

	reader.set_data(c_str);
	pair_cell::cxr cxr;

	cxr = st.read(&reader);
	/*
	if (!is_list(cxr)) {
		throw syntax_error("test case is error");
	}
	*/
	/*
	//printer.println(cxr);
	printer.println(cxr->cdr);
	std::cout << "cxr->ptr.cdr:" << cxr->cdr.value << std::endl;
	std::cout << "cxr->ptr.cdr:" << cxr->cdr.ptr->car.value << std::endl;
	std::cout << "cxr->ptr.cdr:" << cxr->cdr.ptr->car.ptr->car.value << std::endl;
	std::cout << "cxr->ptr.cdr:" << cxr->cdr.ptr->car.ptr->car.ptr->car.value << std::endl;
	std::cout << "cxr->ptr.cdr:" << cxr->cdr.ptr->car.ptr->car.ptr->cdr.value << std::endl;
	*/

	pair_cell::cxr rv;
	switch ( cmd ) {
	case 'd': 
		rv.ptr = st.deep_copy(static_cast<pair_cell *>(cxr));
		break;
	case 'a':
	{
		pair_cell::cxr cxr2;
		cxr2 = st.read(&reader);
		rv = st.append(static_cast<pair_cell *>(cxr), cxr2);
	}
		break;
	case 'f':
		rv.ptr = st.find_last(static_cast<pair_cell *>(cxr));
		break;
	case 'x':
		rv = cxr;
		break;
	default:
		throw syntax_error("test case cmd is error");
	}

	printer.println(rv);

	return;
}

//----------------------------------------------------------------
int
main(int argc, char **argv)
{
	char cmd;
	const char *c_str;

	if ( argc != 3 ) {
		std::cerr << "Usage:scheme_func_test cmd list" << std::endl;
		exit(1);
	}
	cmd = argv[1][0];
	c_str = argv[2];

	scheme_test(cmd, c_str);

	return 0;
}
