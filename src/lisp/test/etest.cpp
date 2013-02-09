#include <iostream>
#include "exception.h"

using namespace sign::scheme;

int
main(int argc, char **argv)
{
	int status;

	status = 0;
retry:
	try {
		switch ( status ) {
		case 0:
			throw fatal_error("my fatal error");
		case 1:
			throw alloc_error("my alloc error");
		case 2:
			throw syntax_error("my sytax error");
		case 3:
			throw eof_exception();
		}

	} catch ( fatal_error &e) {
		std::cout << e.what() << std::endl;
		status = 1;
		goto retry;
	} catch ( alloc_error &e) {
		std::cout << e.what() << std::endl;
		status = 2;
		goto retry;
	} catch ( sytax_error &e) {
		std::cout << e.what() << std::endl;
		status = 3;
		goto retry;
	} catch ( eof_exception &e) {
		std::cout << "EOF" << std::endl;
	}
}
