#ifndef __SIGN_SCHEME_REGISTER_DRIVER_H__
#define __SIGN_SCHEME_REGISTER_DRIVER_H__

#include <string>
#include "sign/scheme++/PrimitiveProcedure.h"

namespace sign {
namespace scheme {

class RegisterDriver {
public:
	struct NameAndLambda {
		const std::string name;
		PrimitiveProcedure::lambda_t lambda;
	};
public:
	RegisterDriver(ExAtom *root_env, const std::string &driver, const std::string *parent_driver = 0, const NameAndLambda *nal = 0);

	volatile int getUseN() const {
		return useN;
	}

private:
	ExAtom *root_env;
	const std::string driver;
	const std::string *parent_driver;
	ExAtom *mama_env;
	ExAtom *new_env;
	int useN;

	void registerAll(const NameAndLambda *);

	ExAtom *searchMamaEnv();
};


} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_REGISTER_DRIVER_H__ */
