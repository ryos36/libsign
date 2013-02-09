#ifndef __SIGN_SCHEME_DISPLAY_H__
#define __SIGN_SCHEME_DISPLAY_H__

#include <string>
#include <ostream>
#include "sign/scheme++/Union.h"

namespace sign {
namespace scheme {

class Display {
public:
	Display(std::ostream *ostream = 0, bool quote_flag = false);

	void print(Union u);
	void newline();

private:
	bool quote_flag;
	std::ostream * const ostream;

	void print_inner(Union a);
	void print_list(Cell *cell);
	void print_cdr(Union a);
};


} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_DISPLAY_H__ */
