#include <iostream>

bool g_is_splicing_body = true;

int
main(int argc, char **argv)
{
	bool *check_is_splicing;
	if ( argc == 1 ) {
		check_is_splicing = 0;
	} else {
		check_is_splicing = &g_is_splicing_body;
	}

	bool is_splicing_body = true;
	bool &is_splicing = check_is_splicing?(*check_is_splicing):is_splicing_body;

	is_splicing = false;

	std::cout << "is_splicing:" << is_splicing << std::endl;
	std::cout << "check_is_splicing:" << check_is_splicing << std::endl;
	std::cout << "*check_is_splicing:" << (check_is_splicing?*check_is_splicing:true) << std::endl;
	std::cout << "g_is_splicing_body:" << g_is_splicing_body << std::endl;
	std::cout << "is_splicing_body:" << is_splicing_body << std::endl;
}
