#ifndef __SIGN_SCHEME_EX_ATOM_H__
#define __SIGN_SCHEME_EX_ATOM_H__

#include <stdint.h>
#include <string>
#include "sign/scheme++/Union.h"

namespace sign {
namespace scheme {

class CellAllocator;
class Union;
class Symbol;
class PrimitiveProcedure;

class ExAtom {
public:
	enum Type {
		_SYMBOL = 0,
		_STRING,
		_CHAR,
		_REAL,
		_EX_NUM,

		_TRUE,
		_FALSE,

		_COMPOUND_PROCEDURE,
		_ENVIRONMENT,

		_PRIMITIVE_PROCEDURE,
		_UNSPECIFIED,

		TYPE_MAX_N,

#define __TYPE_PRIMITIVE_TO_TYPE__(__x__) __x__ = (_ ## __x__ << Union::TAG_SHIFT) | Union::NOT_CELL
		__TYPE_PRIMITIVE_TO_TYPE__(SYMBOL),
		__TYPE_PRIMITIVE_TO_TYPE__(STRING),
		__TYPE_PRIMITIVE_TO_TYPE__(CHAR),
		__TYPE_PRIMITIVE_TO_TYPE__(REAL),
		__TYPE_PRIMITIVE_TO_TYPE__(EX_NUM),

		__TYPE_PRIMITIVE_TO_TYPE__(TRUE),
		__TYPE_PRIMITIVE_TO_TYPE__(FALSE),

		__TYPE_PRIMITIVE_TO_TYPE__(COMPOUND_PROCEDURE),
		__TYPE_PRIMITIVE_TO_TYPE__(ENVIRONMENT),

		__TYPE_PRIMITIVE_TO_TYPE__(PRIMITIVE_PROCEDURE),
		__TYPE_PRIMITIVE_TO_TYPE__(UNSPECIFIED),
#undef __TYPE_PRIMITIVE_TO_TYPE__
	};

private:
	friend class CellAllocator;
	friend class Union;

	ExAtom(Type type, void *ptr);

public:
	Type type;

	void marked();
	bool isMarked() const;
	uint32_t getValue() const { return reinterpret_cast<uint32_t>(ptr); }

public:
	ExAtom *getMarkedExAtom();
	void setMarkedExAtom(ExAtom *);

	Cell *getEnvironmentCell() const;
	Cell *getCompoundProcedureCell() const;
	PrimitiveProcedure *getPrimitiveProcedurePointer();

private:
	void *ptr;

	Symbol *getSymbolPointer() const;
	String *getStringPointer() const;

	char getChar() const;
	double getDouble() const;
	void setDouble(double d);

	ExAtom & operator = (const ExAtom &rhs) {
		if ( this == &rhs ) {
			return *this;
		}
			
		type = rhs.type;
		ptr = rhs.ptr;
		return *this;
	}

public:
	static ExAtom *alloc(Type type, void *ptr);
	static ExAtom *alloc_char(char c);
	static ExAtom *alloc_double(double d);
	static ExAtom *alloc_environment(ExAtom *env);
	static ExAtom *alloc_compound_procedure(ExAtom *env, Cell *args, Cell *body);

	static void release(ExAtom *exatom);
	static ExAtom *find_symbol_as_ExAtom(const std::string &str);

	static ExAtom True;
	static ExAtom False;
	static ExAtom Unspecified;
	static ExAtom Lambda;
	static ExAtom NoEnvironment;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_ATOM_H__ */
