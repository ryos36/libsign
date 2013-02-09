#ifndef __SIGN_SCHEME_ARG_CHECKER_H__
#define __SIGN_SCHEME_ARG_CHECKER_H__

#include <stdexcept>
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/Number.h"

namespace sign {
namespace scheme {

template <class C0R>
Union
cxr(ExAtom *env, Cell *args)
{
	C0R c0r;

	if ( args == Cell::NIL ) {
		return Union(Cell::NIL);
	}
	if (!args->cdr.isNull()) {
		throw std::runtime_error("invalid args count");
	}
	Union arg0;
	arg0 = args->car;
	return c0r.get(arg0);
}

template <class C1R, class C0R>
Union
cxxr(ExAtom *env, Cell *args)
{
	C0R c0r;
	C1R c1r;

	if ( args == Cell::NIL ) {
		return Union(Cell::NIL);
	}
	if (!args->cdr.isNull()) {
		throw std::runtime_error("invalid args count");
	}
	Union arg0;
	arg0 = args->car;
	return c1r.get(c0r.get(arg0));
}

template <class C2R, class C1R, class C0R>
Union
cxxxr(ExAtom *env, Cell *args)
{
	C0R c0r;
	C1R c1r;
	C2R c2r;

	if ( args == Cell::NIL ) {
		return Union(Cell::NIL);
	}
	if (!args->cdr.isNull()) {
		throw std::runtime_error("invalid args count");
	}
	Union arg0;
	arg0 = args->car;
	return c2r.get(c1r.get(c0r.get(arg0)));
}

template <class C3R, class C2R, class C1R, class C0R>
Union
cxxxxr(ExAtom *env, Cell *args)
{
	C0R c0r;
	C1R c1r;
	C2R c2r;
	C3R c3r;

	if ( args == Cell::NIL ) {
		return Union(Cell::NIL);
	}
	if (!args->cdr.isNull()) {
		throw std::runtime_error("invalid args count");
	}
	Union arg0;
	arg0 = args->car;
	return c3r.get(c2r.get(c1r.get(c0r.get(arg0))));
}

class CAR {
public:
	Union get(Union u) {
		if (!u.isCell()) {
			throw std::runtime_error("must be cons");
		}
		if ( u.isNull()) {
			return Union(Cell::NIL);
		}
		return u->car;
	}
};

class CDR {
public:
	Union get(Union u) {
		if (!u.isCell()) {
			throw std::runtime_error("must be cons");
		}
		if ( u.isNull()) {
			return Union(Cell::NIL);
		}
		return u->cdr;
	}
};

class IsNumber {
public:
	void check(Union u) {
		if (!u.isNumber()) {
			throw std::runtime_error("arg must be Number.");
		}
	}

	typedef Number arg_type;

	Number convert(Union u) {
		return Number(u);
	}
};

class IsInteger {
public:
	void check(Union u) {
		if (!u.isInteger()) {
			throw std::runtime_error("arg must be integer.");
		}
	}

	typedef int arg_type;

	int convert(Union u) {
		return static_cast<int>(u);
	}
};

class IsCell ;

class IsExAtom ;

class IsProcedureExAtom {
public:
	void check(Union u) {
		if (!u.isProcedure()) {
			throw std::runtime_error("arg must be procedure.");
		}
	}

	typedef ExAtom *arg_type;

	ExAtom *convert(Union u) {
		return u.getProcedureExAtom();
	}
};

class IsSymbol {
public:
	void check(Union u) {
		if (!u.isSymbol()) {
			throw std::runtime_error("arg must be symbol.");
		}
	}

	typedef Symbol *arg_type;

	Symbol *convert(Union u) {
		return u.getSymbolPointer();
	}
};

class IsEnvironment {
public:
	void check(Union u) {
		if (!u.isEnvironment()) {
			throw std::runtime_error("arg must be environment.");
		}
	}

	typedef ExAtom *arg_type;

	ExAtom *convert(Union u) {
		return u.getEnvironmentExAtom();
	}
};

class IsAny {
public:
	void check(Union u) {
	}

	typedef Union arg_type;

	Union convert(Union u) {
		return u;
	}
};

class IsChar {
public:
	void check(Union u) {
		if (!u.isChar()) {
			throw std::runtime_error("arg must be char.");
		}
	}

	typedef char arg_type;

	char convert(Union u) {
		return u.getChar();
	}
};

class IsString {
public:
	void check(Union u) {
		if (!u.isString()) {
			throw std::runtime_error("arg must be string.");
		}
	}

	typedef String* arg_type;

	String* convert(Union u) {
		return u.getStringPointer();
	}
};


} // namespace scheme
} // namespace sign

//------------------------------------------------------------------------
#define CXXR_BEGIN namespace sign { namespace scheme { namespace primitive
#define CXXR_END  }}
#define CXXXR_BEGIN namespace sign { namespace scheme { namespace primitive
#define CXXXR_END  }}
#define CXXXXR_BEGIN namespace sign { namespace scheme { namespace primitive
#define CXXXXR_END  }}

#define CXXR( N1, N0 ) Union c ## N1 ## N0 ## r (ExAtom *env, Cell *args) \
{ \
	return cxxr<C ## N1 ## R, C ## N0 ## R>(env, args); \
}

#define CXXXR( N2, N1, N0 ) Union c ## N2 ## N1 ## N0 ## r (ExAtom *env, Cell *args) \
{ \
	return cxxxr<C ## N2 ## R, C ## N1 ## R, C ## N0 ## R>(env, args); \
}

#define CXXXXR( N3, N2, N1, N0 ) Union c ## N3 ## N2 ## N1 ## N0 ## r (ExAtom *env, Cell *args) \
{ \
	return cxxxxr<C ## N3 ## R, C ## N2 ## R, C ## N1 ## R, C ## N0 ## R>(env, args); \
}

#endif /* __SIGN_SCHEME_ARG_CHECKER_H__ */
