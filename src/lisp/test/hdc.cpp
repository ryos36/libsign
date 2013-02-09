#include <iostream>
#include "hand_dynamic_cast.h"

using namespace sign::scheme;

int
main()
{
	sign::scheme::hand_dynamic_cast<ExSymbol *, SYMBOL> a;
	pair_cell::cxr cxr;
	ExSymbol *e;
	cxr = long_int_to_cxr(3);
	e = a.func(cxr);
	std::cout << e << std::endl;
	e = cxr.hand_dynamic_cast<ExSymbol *, SYMBOL>();
	std::cout << e << std::endl;
}
