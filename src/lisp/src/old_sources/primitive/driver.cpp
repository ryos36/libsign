#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Environment.h"
#include "sign/scheme++/Debug.h"

#include "sign/scheme++/Cons.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace sign::scheme;

//------------------------------------------------------------------------
Union eval_sequence(ExAtom *env, Cell *seq);
Union apply(ExAtom *env, ExAtom *, Cell *args);

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union _with_open_driver(ExAtom *env, Cell *args)
{
	ArgsIsAtLeast1<IsSymbol> checker(args);
	checker.check();
	
	Symbol *driver_sym;
	driver_sym = checker.get0();

	Cell *body;
	body = checker.getRemains();
	if ( body == Cell::NIL ) {
		throw std::runtime_error("no body.");
	}


	ExAtom *new_env;
	if ( driver_sym->name == "root" ) {
		new_env = environment::getSystemRootEnvironment();
	} else {
		Union u_new_env;
		bool find_flag;
		u_new_env = environment::getValue(env, *driver_sym, &find_flag);
		if ( !find_flag ) {
			throw std::runtime_error("no driver name.");	
		}

		new_env = u_new_env.getEnvironmentExAtom();
		if ( new_env == 0 ) {
			throw std::runtime_error("no driver.");	
		}
	}
	return eval_sequence(new_env, body);
}

//------------------------------------------------------------------------
Union _driver_call(ExAtom *env, Cell *args)
{
	ArgsIsAtLeast2<IsSymbol, IsSymbol> checker(args);
	checker.check();
	
	Symbol *driver_sym;
	driver_sym = checker.get0();

	Symbol *func_sym;
	func_sym = checker.get1();

	Cell *body;
	body = checker.getRemains();

        Union u_new_env;
        bool find_flag;
        u_new_env = environment::getValue(env, *driver_sym, &find_flag);
        if ( !find_flag ) {
		throw std::runtime_error("no driver name.");	
        }

        ExAtom *new_env;
        new_env = u_new_env.getEnvironmentExAtom();
        if ( new_env == 0 ) {
		throw std::runtime_error("no driver.");	
        }

	Union u_func;
        u_func = environment::getValue(new_env, *func_sym, &find_flag);
        if ( !find_flag ) {
		throw std::runtime_error("no func name.");	
	}

	ExAtom *func;
	func = u_func;

	return  apply(new_env, func, body);
}

} // namespace primitive
} // namespace scheme
} // namespace sign
