#include <string>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Symbol.h"
#include "sign/scheme++/Environment.h"
#include "sign/scheme++/PrimitiveProcedure.h"
#include "sign/scheme++/RegisterPrimitive.h"

using namespace sign::scheme;

RegisterPrimitive::RegisterPrimitive(ExAtom *env, const NameAndLambda *nal)
	: env(env), useN(0)
{
	if ( nal ) {
		registerAll(nal);
	}
}

//------------------------------------------------------------------------
void
RegisterPrimitive::operator = (const NameAndLambda *nal)
{
	registerAll(nal);
}

//------------------------------------------------------------------------
void
RegisterPrimitive::registerAll(const NameAndLambda *nal)
{
	while( nal->lambda ) {
		PrimitiveProcedure *pp;
		pp = new PrimitiveProcedure(nal->name, nal->lambda);
		ExAtom *exAtom;
		exAtom = ExAtom::alloc(ExAtom::PRIMITIVE_PROCEDURE, pp);
		Symbol *symbol;
		symbol = new Symbol(pp->name);

		Union value(exAtom);
		environment::intern(env, symbol, value);

		++nal;
		++useN;
	}
}
