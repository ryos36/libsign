#ifndef __SIGN_SCHEME_UNION_H__
#define __SIGN_SCHEME_UNION_H__

#include "sign/scheme++/String.h"

namespace sign {
namespace scheme {

class Cell;
class ExAtom;
class Symbol;
class PrimitiveProcedure;

class Union {
public:
	enum Tag {
		CELL_POINTER = 0x0,
		ATOM = 0x1,
		SHORT_NUM = 0x2,
		NOT_CELL = 0x4,

		EX_ATOM = ATOM, 
		ATOM_SHORT_NUM = ATOM | SHORT_NUM,
		MARKED_CELL_POINTER = 0x2,
		SELF_EX_ATOM = NOT_CELL,

		TAG_MASK = 0x7,
		TAG_SHIFT = 3,
		MINUS_MASK_VALUE = ((0xFFFFFFFF >> (32 - TAG_SHIFT - 1)) << (32 - TAG_SHIFT - 1))
	};

public:
	explicit Union(Cell *cell = 0);
	explicit Union(int);
	explicit Union(ExAtom *ex_atom);
	explicit Union(bool);

public:
	Tag getTag() const;

	bool isNull() const;
	bool isAtom() const;
	bool isCell() const;
	bool isExAtom() const;

	bool isSelfExAtom() const;

	bool isNumber() const;
	bool isInteger() const;
	bool isChar() const;
	bool isString() const;
	bool isSymbol() const;
	bool isEnvironment() const;
	bool isProcedure() const;
	bool isBoolean() const;

	bool isFalse() const;
	bool isPair() const;
	bool isTaggedList(const Symbol *) const;

	bool eq_p(const Union &u) const;
	bool eqv_p(const Union &u) const;
	bool equal_p(const Union &u) const;
public:
	operator int () const;
	int operator = (const int new_value);

	operator ExAtom *() const;
	ExAtom * operator = (ExAtom * const new_ex_atom);

	operator Cell *() const;
	Cell * operator = (Cell * const new_cell_p);
	Cell *operator ->() const;

#ifdef DEBUG
	unsigned int getValue() const {
		return value;
	}
#endif

public:
	Symbol *getSymbolPointer() const;
	String *getStringPointer() const;
	//PrimitiveProcedure *getPrimitiveProcedurePointer() const;
	char getChar() const;
	double getDouble() const;
	void setDouble(double d);

	ExAtom *getEnvironmentExAtom();
	ExAtom *getProcedureExAtom();

	void marked();
	bool isMarked() const;

private:
	static ExAtom *toExAtom(unsigned int va) {
		return reinterpret_cast<ExAtom *>(va & ~TAG_MASK);
	}
	

public:
	static Union deep_copy(const Union u);
	static Union list(Union u0, Union u1);
	static Union cons(Union u0, Union u1);

	static Union NIL;
	static Union True;
	static Union False;
	static Union Unspecified;
private:
	unsigned int value;
};

//------------------------------------------------------------------------
inline
Union::Tag
Union::getTag() const
{
	int tag;
	tag = value & TAG_MASK;
	return (Union::Tag)tag;
}

//------------------------------------------------------------------------

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_UNION_H__ */
