#ifndef __SIGN_SCHEME_REGISTER_PRIMITIVE_H__
#define __SIGN_SCHEME_REGISTER_PRIMITIVE_H__

#include <string>
#include "sign/scheme++/PrimitiveProcedure.h"

namespace sign {
namespace scheme {

class RegisterPrimitive {
public:
	struct NameAndLambda {
		const std::string name;
		PrimitiveProcedure::lambda_t lambda;
	};
public:
	RegisterPrimitive(ExAtom *env, const NameAndLambda *nal = 0);

	void operator = (const NameAndLambda *);

	volatile int getUseN() const {
		return useN;
	}

private:
	ExAtom *env;
	int useN;

	void registerAll(const NameAndLambda *);
};


} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_REGISTER_PRIMITIVE_H__ */
