#include <string>
#include "sign/sign_assert.h"
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Symbol.h"
#include "sign/scheme++/Environment.h"
#include "sign/scheme++/PrimitiveProcedure.h"
#include "sign/scheme++/RegisterDriver.h"

using namespace sign::scheme;

//----------------------------------------------------------------
RegisterDriver::RegisterDriver(ExAtom *env, const std::string &driver, const std::string *parent_driver, const NameAndLambda *nal)
	: root_env(env), driver(driver), parent_driver(parent_driver), useN(0)
{
	if ( parent_driver ) {
		mama_env = searchMamaEnv();
	} else {
		mama_env = env;
	}

	new_env = ExAtom::alloc_environment(mama_env);
	SIGN_ASSERT(new_env);

	Symbol *symbol;
	symbol = new Symbol(driver);

	Union value(new_env);
	environment::intern(env, symbol, value);

	if ( nal ) {
		registerAll(nal);
	}
}

//------------------------------------------------------------------------
void
RegisterDriver::registerAll(const NameAndLambda *nal)
{
	while( nal->lambda ) {
		PrimitiveProcedure *pp;
		pp = new PrimitiveProcedure(nal->name, nal->lambda);
		ExAtom *exAtom;
		exAtom = ExAtom::alloc(ExAtom::PRIMITIVE_PROCEDURE, pp);
		Symbol *symbol;
		symbol = new Symbol(pp->name);

		Union value(exAtom);
		environment::intern(new_env, symbol, value);

		++nal;
		++useN;
	}
}

//----------------------------------------------------------------
ExAtom *
RegisterDriver::searchMamaEnv()
{
	bool find_flag;
	Union s_env;

	Symbol symbol(*parent_driver);
	s_env = environment::getValue(root_env, symbol, &find_flag);
	SIGN_ASSERT(find_flag);

	return s_env;
}

