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

void
scheme_test(const char *c_str)
{
	Scheme scheme0(1024);
	Scheme scheme1(1024);

	scheme0.init();
	scheme1.init();

	SimpleTokenReader reader;
	reader.set_data(c_str);
	Printer printer(std::cout);
	scheme0.echo_on();
	scheme0.repl(&reader, &printer);
}


//----------------------------------------------------------------
int
main(int argc, char **argv)
{
	const char *c_str;

	if ( argc == 1 ) {
		c_str = "( a b c )";
	} else {
		c_str = argv[1];
	}
	scheme_test(c_str);

	return 0;
}
