#include <iostream>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#define P_DEBUG
#include "PairCellAllocator.h"
#include "Environment.h"
#include "Scheme.h"

using namespace sign::scheme;

#if 0
void
scheme_test()
{
	Scheme scheme0(1024);
	Scheme scheme1(1024);

	scheme0.init();
	scheme1.init();

	pair_cell::cxr v, rv;
	v.value = ATOM_SHORT_NUM | 0x12090000;
	rv = scheme0.eval(v);
	std::cout << std::hex << "rv:" << rv.value << std::dec << std::endl;

	v.value = ATOM_SHORT_NUM | 0x11030000;
	scheme0.intern("value", v);

	PairCellAllocator pcp(1024);
	pcp.init();

	ExSymbol *symbol;
	symbol = ExSymbol::alloc_ExSymbol(pcp, "value");

	v = reinterpret_cast<pair_cell *>(symbol);
	rv = scheme0.eval(v);
	std::cout << std::hex << "value rv:" << rv.value << std::dec << std::endl;
}
#endif

#if 0
//----------------------------------------------------------------
void
gc_test_with_root()
{
	PairCellAllocator pcp(1024);
	std::cout << "start init." << std::endl;
	pcp.init();
	std::cout << "init done." << std::endl;

	pair_cell *pcell;
	pcell = pcp.alloc();
	pcell->car.value = ATOM_SHORT_NUM;
	pcell->cdr.value = ATOM_SHORT_NUM;
	// but it's garbage

	Environment root_env(pcp);

	std::string sym("symbol");
	pcell = pcp.alloc(SYMBOL, &sym);
	// but it's garbage

	std::string sym2("symbol2");
	pair_cell::cxr v0;
	v0.value = ATOM_SHORT_NUM;

	root_env.intern(sym2, v0);

	/*
	try {
		while ( 1 ) {
			pcp.alloc();
		}
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}
	pcp.re_init();
	std::cout << "=======ok==========" << std::endl;

	std::string sym3("symbol3");
	root_env.intern(&sym3, v0);
	pcell = pcp.alloc(SYMBOL, &sym);

	pcp.re_init();

	try {
		root_env.intern(&sym3, v0);
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}

	try {
		pcp.alloc(STRING, "forever", 1024 * 1024);
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}

	pcp.alloc(STRING, "forever", 24);
	*/

	std::string sym4("symbol4");
	pair_cell *p;
	p = pcp.alloc(STRING, "forever", 24);
	pair_cell::cxr v;

	v = p;
	std::cout << "v:" << std::hex << v.value << std::endl;
	std::cout << "car:" << std::hex << static_cast<pair_cell *>(v)->car.value << std::endl;
	std::cout << "cdr:" << std::hex << static_cast<pair_cell *>(v)->cdr.value << std::endl;
	root_env.intern(sym4, v);
	pcp.re_init();

	bool find_flag;
	v = root_env.getValue(sym4, &find_flag);
	if ( find_flag ) {
		std::cout << "find:" << std::hex << v.value << std::endl;
		std::cout << "car:" << std::hex << static_cast<pair_cell *>(v)->car.value << std::endl;
		std::cout << "cdr:" << std::hex << static_cast<pair_cell *>(v)->cdr.value << std::endl;
	}

	std::string sym5("symbol5");
	v = root_env.getValue(sym5, &find_flag);
	if ( !find_flag ) {
		std::cout << "not find:" << std::endl;
	}
	v0.value = ATOM_SHORT_NUM | 0x12090000;
	root_env.intern(sym5, v0);
	v = root_env.getValue(sym5, &find_flag);
	if ( find_flag ) {
		std::cout << "find:" << std::hex << v.value << std::endl;
	}
}
#endif

//----------------------------------------------------------------
void
gc_test()
{
	PairCellAllocator pcp(1024);
	std::cout << "start init." << std::endl;
	pcp.init();
	std::cout << "init done." << std::endl;

	pair_cell *pcell;
	pcell = pcp.alloc();
	pcell->car.value = ATOM_SHORT_NUM;
	pcell->cdr.value = ATOM_SHORT_NUM;
	// but it's garbage

	Environment root_env(pcp);

	ExSymbol *ex_symbol0;
	ex_symbol0 = ExSymbol::alloc_ExSymbol(pcp, "symbol0");
	std::cout << "ex_symbol0:" << ex_symbol0->get_c_str() << std::endl;
	// but it's garbage

	ExSymbol *ex_symbol1;
	ex_symbol1 = ExSymbol::alloc_ExSymbol(pcp, "symbol1");
	pair_cell::cxr v0;
	v0.value = ATOM_SHORT_NUM;

	root_env.intern(ex_symbol1, v0);

	/*
	try {
		while ( 1 ) {
			pcp.alloc();
		}
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}
	pcp.re_init();
	std::cout << "=======ok==========" << std::endl;

	std::string sym3("symbol3");
	root_env.intern(&sym3, v0);
	pcell = pcp.alloc(SYMBOL, &sym);

	pcp.re_init();

	try {
		root_env.intern(&sym3, v0);
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}

	try {
		pcp.alloc(STRING, "forever", 1024 * 1024);
	} catch ( std::runtime_error &error ) {
		std::cout << error.what() << std::endl;
	}

	pcp.alloc(STRING, "forever", 24);
	*/

	pair_cell *p;
	p = pcp.alloc(STRING, "forever", 24);
	pair_cell::cxr v;

	v = p;
	std::cout << "v:" << std::hex << v.value << std::endl;
	std::cout << "car:" << std::hex << static_cast<pair_cell *>(v)->car.value << std::endl;
	std::cout << "cdr:" << std::hex << static_cast<pair_cell *>(v)->cdr.value << std::endl;

	ExSymbol *ex_symbol4;
	ex_symbol4 = ExSymbol::alloc_ExSymbol(pcp, "symbol4");

	root_env.intern(ex_symbol4, v);
	pcp.re_init();

	bool find_flag;
	v = root_env.getValue(ex_symbol4, &find_flag);
	if ( find_flag ) {
		std::cout << "find:" << std::hex << v.value << std::endl;
		std::cout << "car:" << std::hex << static_cast<pair_cell *>(v)->car.value << std::endl;
		std::cout << "cdr:" << std::hex << static_cast<pair_cell *>(v)->cdr.value << std::endl;
	}

	ExSymbol *ex_symbol5;
	ex_symbol5 = ExSymbol::alloc_ExSymbol(pcp, "symbol5");

	v = root_env.getValue(ex_symbol5, &find_flag);
	if ( !find_flag ) {
		std::cout << "not find:" << std::endl;
	}
	v0.value = ATOM_SHORT_NUM | 0x12090000;
	root_env.intern(ex_symbol5, v0);
	v = root_env.getValue(ex_symbol5, &find_flag);
	if ( find_flag ) {
		std::cout << "find:" << std::hex << v.value << std::endl;
	}

	{
		pair_cell::cxr p;
		pair_cell::cxr v;
		p = static_cast<pair_cell *>(pcp.alloc());
		p->car.value = 0x1209;
		p->cdr.value = 0x1103;

		std::cout << "p car:" << (p.ptr->car.value) << std::endl;
		std::cout << "p cdr:" << (p.ptr->cdr.value) << std::endl;

		v.value = ATOM_SHORT_NUM | 0x12090000;
		std::cout << "v    :" << (v.value) << std::endl;
		/*
		std::cout << "v   p:" << (v->car.value) << std::endl;
		assert!!
		*/

		std::cout << "dcast fuu v:" << static_cast<const ExSymbol *>(p) << std::endl;
		std::cout << "dcast fuu v:" << static_cast<const ExSymbol *>(v) << std::endl;
	}
	{
		pair_cell::cxr v;
		
		v = reinterpret_cast<pair_cell *>(ex_symbol5);
		std::cout << "dcast fuu v:" << static_cast<const ExSymbol *>(v) << std::endl;
	}

}

//----------------------------------------------------------------
int
main(int argc, char **argv)
{
	gc_test();
	//scheme_test();

	return 0;
}
