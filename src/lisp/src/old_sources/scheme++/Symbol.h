#ifndef __SIGN_SCHEME_SYMBOL_H__
#define __SIGN_SCHEME_SYMBOL_H__

#include <string>
#include "sign/sign_assert.h"

namespace sign {
namespace scheme {

class ExAtom;
class Symbol {
public:
	Symbol(const std::string name);

	bool operator < (const Symbol &sym) const {
		return this->name < sym.name;
	}

	bool operator == (const Symbol &sym) const;

	const std::string &operator *() const;
private:
	friend class ExAtom;
	Symbol(const Symbol &symbol);

public:
	const std::string name;
};


} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_SYMBOL_H__ */
