#include <iostream>
#include <cstdlib>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "sign/RCPointer.h"
#define P_DEBUG
#include "PairCellAllocator.h"
#include "Environment.h"
#include "Number.h"
#include <boost/lexical_cast.hpp>

using namespace sign;
using namespace sign::scheme;

void
num_test()
{
	PairCellAllocator allocator(1024);
	std::cout << "start init." << std::endl;
	allocator.init();
	std::cout << "init done." << std::endl;
	RCPointer<Number> num;

	num = Number::new_number(allocator, "0");
	std::cout << *num << std::endl;

	/*
	pair_cell::cxr cxr;
	cxr = static_cast<int>(1);
	std::cout << "tag:" << ((1 & 0x0FFFFFFF) << TAG_SHIFT) << std::endl;
	*/
	num = Number::new_number(allocator, "1");
	std::cout << "1:" << *num << std::endl;
	long int v;
	v = strtoll("0xc8000000", 0, 16);
	std::cout << "v:" << v << std::endl;

	num = Number::new_number(allocator, "-1");
	std::cout << "-1:" << *num << std::endl;

	//num = Number::new_number(allocator, boost::lexical_cast<std::string>(v));
	std::cout << "str:" << boost::lexical_cast<long int>("3355443200") << std::endl;
	num = Number::new_number(allocator, "3355443200");
	std::cout << "hex:" << std::hex << *num << std::dec << std::endl;

	num = Number::new_number(allocator, "3.14159");
	std::cout << "double:" << *num << std::endl;
}

//----------------------------------------------------------------
int
main(int argc, char **argv)
{
	num_test();

	return 0;
}
