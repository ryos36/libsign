#ifndef __SIGN_SCHEME_PRIMITIVE_PROCEDURE_H__
#define __SIGN_SCHEME_PRIMITIVE_PROCEDURE_H__

#include <string>
#include "sign/scheme++/Union.h"

namespace sign {
namespace scheme {
class ExAtom;
class Cell;

class PrimitiveProcedure {
public:
	typedef Union (*lambda_t) (ExAtom *env, Cell *args);
public:
	PrimitiveProcedure(const std::string name, lambda_t lambda);
	Union apply(ExAtom *env, Cell *args);

public:
	const std::string name;
	lambda_t lambda;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_PRIMITIVE_PROCEDURE_H__ */
