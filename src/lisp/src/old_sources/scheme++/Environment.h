#ifndef __SIGN_SCHEME_ENVIRONMENT_H__
#define __SIGN_SCHEME_ENVIRONMENT_H__

#include "sign/scheme++/Symbol.h"
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"

namespace sign {
namespace scheme {
namespace environment {
	ExAtom *alloc(ExAtom *env);
	ExAtom *getSystemRootEnvironment();

	void extend(ExAtom *new_env, Cell *args, Cell *values);

	bool setValue(ExAtom *env, const Symbol &symbol, Union &value);
	void defineValue(ExAtom *env, const Symbol &symbol, Union value);
	Union getValue(ExAtom *env, const Symbol &symbol, bool *find_flag = 0);

	void intern(ExAtom *env, Symbol *symbol, Union value);
} // namespace environment
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_ENVIRONMENT_H__ */
