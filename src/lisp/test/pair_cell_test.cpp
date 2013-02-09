#include <iostream>
#include "pair_cell.h"
#include "sign/sign_assert.h"
#include "PairCellAllocator.h"

using namespace sign::scheme;

int
main(int argc, char **argv)
{
	pair_cell p0, p1, p2;

	p0.car.ptr = &p1;
	p0.cdr.ptr = &p2;

	std::cout << p0.car.value << std::endl;	
	std::cout << p0.cdr.value << std::endl;	

	PairCellAllocator pcp(1024);
	std::cout << "start init." << std::endl;
	pcp.init();
	std::cout << "init done." << std::endl;

	pair_cell *p;
	int n = 0;
	do {
		++n;
		p = pcp.alloc();
	} while ( p );
	std::cout << n << std::endl;
	SIGN_ASSERT( n == (1024 + 1));
}
