/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_PAIR_CELL_H__
#define __SIGN_SCHEME_PAIR_CELL_H__

#include <stdint.h>
#include "sign/sign_assert.h"

namespace sign {
namespace scheme {

class ExSymbol;
class ExString;
class ExChar;
class ExExNum;
class ExBigNum;
class ExPointer;
class ExAllocator;

struct pair_cell {
	union cxr {
		unsigned long value;
		pair_cell *ptr;

		void *ex_atom; // use only cdr

		cxr &operator= (pair_cell *_ptr);
		explicit operator pair_cell *();
		pair_cell *operator->();

		/* hand dynamic cast */
		explicit operator ExSymbol *();
		explicit operator ExString *();
		explicit operator ExChar *();
		explicit operator ExExNum *();
		explicit operator ExBigNum *();
		explicit operator ExPointer *();
		explicit operator ExAllocator *();
		explicit operator long int();

		template<class X, int S>
		X hand_dynamic_cast();
		template<class X>
		X hand_dynamic_cast();
	};
	union cxr car;
	union cxr cdr;

	/*
	32bit
	xxxx xxxx xxxx xxxx xxxx xxxx xxxx xTTT 

	64bit
	xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx 
	xxxx xxxx xxxx xxxx xxxx xxxx xxxx xTTT 

	TTT means Tag
	*/

	enum {
		CONTINOUS_BLOCK = 1
	};
};

//----------------------------------------------------------------

enum Tag {
	TAG_POINTER       = 0x0, // include ex_atom pointer
	ATOM_FLAG         = 0x1,
	SHORT_NUM_FLAG    = 0x2,
	NOT_CELL_FLAG     = 0x4,

	MARKED_FLAG       = 0x2,

	ATOM_SHORT_NUM    = ATOM_FLAG | SHORT_NUM_FLAG,
	EX_ATOM_ENTITY    = NOT_CELL_FLAG,
	ILLEGAL_TAG       = 0x7,

	TAG_MASK          = 0x7UL,
	TAG_SHIFT         = 0x3,
	
	LENGTH_SHIFT      = 0x4,

	LONG_INT_BIT_SIZE = ((sizeof(long int) == 8)?64:32),
	MINUS_BIT = (0x1UL << (LONG_INT_BIT_SIZE - 1)),

	SHORT_NUM_MASK = ~TAG_MASK,
	SHORT_ABS_NUM_MASK = (SHORT_NUM_MASK & ~MINUS_BIT),
	MINUS_MASK = SHORT_NUM_MASK & ~( SHORT_NUM_MASK >> TAG_SHIFT ),
	ABS_NUM_MASK = (SHORT_NUM_MASK & ~MINUS_BIT) >> TAG_SHIFT,
};

//----------------------------------------------------------------
enum ExAtomType {
	_SYMBOL = 0,
	_STRING,
	_CHAR,
	_REAL,
	_EX_NUM,
	_BIG_NUM,

	_TRUE,
	_DEPRECATED_FALSE,

	_ROOT,
	_ALLOCATOR,

	_DEPRECATED_COMPOUND_PROCEDURE,  // for SICP scheme/ going to deprecated
	_DEPRECATED_PRIMITIVE_PROCEDURE, // for SICP scheme/ going to deprecated

	_SPECIAL_OPERATOR,
	_SYSTEM_FUNCTION,
	_FUNCTION,
	_COMPILED_FUNCTION,
	_MACRO,

	_UNSPECIFIED,

	EX_ATOM_TYPE_MAX_N,

	HAS_SUCCESSOR_PAIR_CELL_FLAG = 0x80000000,
	HAS_CONTINUOUS_N_FLAG        = 0x40000000,
	SPECIAL_FLAG_MASK            = 0xC0000000,
	SPECIAL_FLAG_SHIFT           = 28,

#define __TYPE_PRIMITIVE_TO_TYPE__(__x__) __x__ = ((_ ## __x__ << TAG_SHIFT) | EX_ATOM_ENTITY)
	__TYPE_PRIMITIVE_TO_TYPE__(SYMBOL) | HAS_CONTINUOUS_N_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(STRING) | HAS_CONTINUOUS_N_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(CHAR),
	__TYPE_PRIMITIVE_TO_TYPE__(REAL) | HAS_CONTINUOUS_N_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(EX_NUM),
	__TYPE_PRIMITIVE_TO_TYPE__(BIG_NUM) | HAS_CONTINUOUS_N_FLAG,

	__TYPE_PRIMITIVE_TO_TYPE__(TRUE),
	__TYPE_PRIMITIVE_TO_TYPE__(DEPRECATED_FALSE),

	__TYPE_PRIMITIVE_TO_TYPE__(ROOT),
	__TYPE_PRIMITIVE_TO_TYPE__(ALLOCATOR),

	__TYPE_PRIMITIVE_TO_TYPE__(DEPRECATED_COMPOUND_PROCEDURE) | HAS_SUCCESSOR_PAIR_CELL_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(DEPRECATED_PRIMITIVE_PROCEDURE),

	__TYPE_PRIMITIVE_TO_TYPE__(SPECIAL_OPERATOR) | HAS_SUCCESSOR_PAIR_CELL_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(SYSTEM_FUNCTION) | HAS_SUCCESSOR_PAIR_CELL_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(FUNCTION) | HAS_SUCCESSOR_PAIR_CELL_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(COMPILED_FUNCTION) | HAS_SUCCESSOR_PAIR_CELL_FLAG,
	__TYPE_PRIMITIVE_TO_TYPE__(MACRO) | HAS_SUCCESSOR_PAIR_CELL_FLAG,

	__TYPE_PRIMITIVE_TO_TYPE__(UNSPECIFIED),
#undef __TYPE_PRIMITIVE_TO_TYPE__
};

//----------------------------------------------------------------

#include "pair_cell_utils.h"
#include "pair_cell_gc_utils.h"

//----------------------------------------------------------------
inline
pair_cell::cxr& pair_cell::cxr::operator= (pair_cell *_ptr)
{
	ptr = _ptr;
	return *this;
}
//----------------------------------------------------------------
inline
pair_cell::cxr
long_int_to_cxr(long int v)
{
	pair_cell::cxr rv;
	long int minus, over_flow;
	minus = v & MINUS_BIT;
	over_flow = v & MINUS_MASK;
	SIGN_ASSERT((over_flow == 0) || ((over_flow & MINUS_MASK) == MINUS_MASK));
	rv.value = (v & ABS_NUM_MASK) << TAG_SHIFT;
	rv.value |= minus;
	rv.value |= ATOM_SHORT_NUM;

	return rv;
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator pair_cell *()
{
	SIGN_ASSERT((value & TAG_MASK) == 0);
	return ptr;
}

//----------------------------------------------------------------
inline
pair_cell *
pair_cell::cxr::operator->()
{
	SIGN_ASSERT(is_pointer(*this)) ;
	return static_cast<pair_cell *>(*this);
}

//----------------------------------------------------------------
template<class X, int S>
X pair_cell::cxr::hand_dynamic_cast()
{
	if (!is_pointer(*this)) {
		return 0;
	}
	if (is_pair_cell_pointer(*this)) {
		return 0;
	}
	SIGN_ASSERT(is_ex_atom_entity_pointer(*this));
	pair_cell *p;
	p = static_cast<pair_cell *>(*this);
	if ( p->car.value != S ) {
		return 0;
	}
	return reinterpret_cast<X>(p);
}

//----------------------------------------------------------------
template<class X>
X pair_cell::cxr::hand_dynamic_cast()
{
	if (!is_pointer(*this)) {
		return 0;
	}
	if (is_pair_cell_pointer(*this)) {
		return 0;
	}
	SIGN_ASSERT(is_ex_atom_entity_pointer(*this));
	pair_cell *p;
	p = static_cast<pair_cell *>(*this);
	return reinterpret_cast<X>(p);
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExSymbol *()
{
	return hand_dynamic_cast<ExSymbol *, SYMBOL>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExString *()
{
	return hand_dynamic_cast<ExString *, STRING>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExChar *()
{
	return hand_dynamic_cast<ExChar *, CHAR>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExExNum *()
{
	return hand_dynamic_cast<ExExNum *, EX_NUM>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExBigNum *()
{
	return hand_dynamic_cast<ExBigNum *, BIG_NUM>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExPointer *()
{
	return hand_dynamic_cast<ExPointer *>();
}

//----------------------------------------------------------------
inline
pair_cell::cxr::operator ExAllocator *()
{
	return hand_dynamic_cast<ExAllocator *, ALLOCATOR>();
}


//----------------------------------------------------------------
inline
pair_cell::cxr::operator long int ()
{
	SIGN_ASSERT((this->value & TAG_MASK) == ATOM_SHORT_NUM);

	int minus, v;

	v = (this->value & SHORT_ABS_NUM_MASK) >> TAG_SHIFT;
	minus = this->value & MINUS_BIT;
	if ( minus ) {
		v |= MINUS_MASK;
	}
	
	return v;
}

//----------------------------------------------------------------

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_PAIR_CELL_H__ */
