#include <string>
#include "sign/scheme++/PrimitiveProcedure.h"

using namespace sign::scheme;

//------------------------------------------------------------------------
PrimitiveProcedure::PrimitiveProcedure(const std::string name, PrimitiveProcedure::lambda_t lambda)
	: name(name), lambda(lambda)
{
	// nothing to do
}

//------------------------------------------------------------------------
Union
PrimitiveProcedure::apply(ExAtom *env, Cell *args)
{
	return (*lambda)(env, args);
}
