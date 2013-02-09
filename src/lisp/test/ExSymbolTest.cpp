#include <iostream>
#include "ExSymbol.h"
#include "PairCellAllocator.h"

using namespace sign::scheme;

int
main(int argc, char **argv)
{
	PairCellAllocator pcp(1024);
	pcp.init();
	ExSymbol *ex_symbol0;
	ExSymbol *ex_symbol1;

	ex_symbol0 = ExSymbol::alloc_ExSymbol(pcp, "abc");

	std::cout << "x ex_symbol tag:" << std::hex << ex_symbol0->get_tag() << std::dec << std::endl;
	std::cout << "ex_symbol c_str:" << ex_symbol0->get_c_str() << std::endl;
	std::cout << "size:" << sizeof(*ex_symbol0) << std::endl;
	std::cout << "size:" << sizeof(pair_cell) << std::endl;

	ex_symbol1 = ExSymbol::alloc_ExSymbol(pcp, static_cast<const char *>(*ex_symbol0));
	std::cout << "ex_symbol1 c_str:" << ex_symbol1->get_c_str() << std::endl;
	if ( *ex_symbol1 == *ex_symbol0 ) {
		std::cout << "ex_symbol is same" << std::endl;
	}
}
