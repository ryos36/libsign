#ifndef __SIGN_SCHEME_CELL_H__
#define __SIGN_SCHEME_CELL_H__

#include "sign/scheme++/Union.h"

namespace sign {
namespace scheme {

class Cell {
protected:
	Cell();

public:
	Union car;
	Union cdr;

	int list_length() const;
	bool isSelfExAtom() const;
public:
	static Cell *alloc();
	static Cell *deep_copy(const Cell *src);

	static Cell *NIL;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_CELL_H__ */
