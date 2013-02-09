#include <stdexcept>
#include <vector>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/ArgChecker.h"
#include "sign/scheme++/Cons.h"
#include "sign/scheme++/Debug.h"

using namespace sign::scheme;

Union apply(ExAtom *env, ExAtom *proc, Cell *args);
//------------------------------------------------------------------------

namespace  {
typedef std::vector<Cell *> CellVector;

void
check_each_arg(Union u, CellVector &cell_vector)
{
	if (!u.isCell()) {
		throw std::runtime_error("arg must be cons");
	}
	if (u.isNull()) {
		throw std::runtime_error("arg must be not NIL");
	}
	cell_vector.push_back(static_cast<Cell *>(u));
}

//------------------------------------------------------------------------
Cell *
make_args(CellVector &cell_vector)
{
	CellVector::iterator iter;
	Cons cons;

	for(iter = cell_vector.begin(); iter != cell_vector.end(); ++iter ) {
		Cell *cell, *next;

		cell = *iter;
		if ( cell == Cell::NIL ) {
			throw std::runtime_error("xinvalid args");
		}
		if (!cell->cdr.isCell()) {
			throw std::runtime_error("invalid list");
		}
		next = static_cast<Cell *>(cell->cdr);

		cons += cell->car;
		*iter = next;
	}

	return cons.getTopCell();
}

//------------------------------------------------------------------------
bool
empty(CellVector &cell_vector)
{
	CellVector::iterator iter;
	bool empty_flag;
	empty_flag = false;
	Cell *cell;

	cell = cell_vector[0];
	empty_flag = ( cell == Cell::NIL ) ;
	if (!empty_flag) {
		return false;
	}

	for(iter = cell_vector.begin(); iter != cell_vector.end(); ++iter ) {
		cell = *iter;
		if ( cell != Cell::NIL ) {
			throw std::runtime_error("illegal args list");
		}
	}

	return true;
}

//------------------------------------------------------------------------
void
procedure_op(Union u, ExAtom *procedure, ExAtom *env)
{
	if (!u.isCell()) {
		throw std::runtime_error("illegal args list");
	}
	
	apply(env, procedure, static_cast<Cell *>(u));
}

} // namespace

//------------------------------------------------------------------------
namespace sign {
namespace scheme {
namespace primitive {

Union scheme_map(ExAtom *env, Cell *args)
{
	Cell *arg0;
	arg0 = args;
	if (arg0 == Cell::NIL) {
		throw std::runtime_error("must be cons");
	}
	if (arg0->cdr.isNull()) {
		throw std::runtime_error("it requires exactly at least 2 argument");
	}
	Union procedure;
	procedure = arg0->car;
	if (!procedure.isProcedure() ) {
		throw std::runtime_error("it requires procedure");
	}

	Union arg1_ = arg0->cdr;
	if (!arg1_->cdr.isCell()) {
		throw std::runtime_error("it requires exactly at least 2 argument");
	}

	Cons cons(arg1_);
	CellVector cell_vector;

	std::for_each(cons.begin(), cons.end(), boost::bind(&check_each_arg, _1, boost::ref(cell_vector)));

	Cons rv_cons;
	Cell *args_for_procedure;

	do {
		args_for_procedure = make_args(cell_vector);
		rv_cons += apply(env, procedure.getProcedureExAtom(), args_for_procedure);
	} while(!empty(cell_vector));

	return Union(rv_cons.getTopCell());
}

//------------------------------------------------------------------------
Union scheme_for_each(ExAtom *env, Cell *args)
{
	ArgsIs2<IsProcedureExAtom, IsCell> checker(args);
	checker.check();

	ExAtom *procedure;
	procedure = checker.get0();

	Cell *cell;
	cell = checker.get1();

	Cons cons(cell);
	std::for_each(cons.begin(), cons.end(), boost::bind(&procedure_op, _1, procedure, env));

	return Union(&ExAtom::Unspecified);
}

} // namespace primitive
} // namespace scheme
} // namespace sign
