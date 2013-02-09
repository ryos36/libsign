#ifndef __SIGN_SCHEME_CONS_CHECKER_H__
#define __SIGN_SCHEME_CONS_CHECKER_H__

/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#include <stdexcept>
#include "pair_cell.h"
#include "PairCellAllocator.h"
#include "ExAllocator.h"
#include "iostream"

namespace sign {
namespace scheme {

//------------------------------------------------------------------------
template<class T0>
class ConsMustBeAtLeast1 {
public:
	ConsMustBeAtLeast1(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		checker.check(arg0->car);
	}

	typedef typename T0::arg_type arg0_type;

	arg0_type get0() {
		return checker.convert(arg0->car);
	}

	pair_cell::cxr getRemains() {
		return arg0->cdr;
	}

private:
	pair_cell * const arg0;
	T0 checker;
};

//------------------------------------------------------------------------
template<class T0, class T1>
class ConsMustBeAtLeast2 {
public:
	ConsMustBeAtLeast2(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_pair_cell_pointer(arg0->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker0.check(arg0->car);

		pair_cell *arg1 = arg0->cdr;
		if (is_nil(arg1)) {
			throw std::runtime_error("must be cons");
		}

		checker1.check(arg1->car);
	}

	typedef typename T0::arg_type arg0_type;
	typedef typename T1::arg_type arg1_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	arg1_type get1() {
		return checker1.convert(arg0->cdr.ptr->car);
	}

	pair_cell::cxr getRemains() {
		return arg0->cdr.ptr->cdr;
	}

private:
	pair_cell * const arg0;
	T0 checker0;
	T1 checker1;
};

//------------------------------------------------------------------------
template<class T0, class T1, class T2>
class ConsMustBeAtLeast3 {
public:
	ConsMustBeAtLeast3(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_pair_cell_pointer(arg0->cdr)) {
			throw std::runtime_error("must be cons");
		}
		checker0.check(arg0->car);

		pair_cell *arg1 = arg0->cdr;
		if (is_nil(arg1)) {
			throw std::runtime_error("it requires exactly 3 argument");
		}
		if (!is_pair_cell_pointer(arg1->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker1.check(arg1->car);

		pair_cell *arg2 = arg1->cdr;
		if (is_nil(arg2)) {
			throw std::runtime_error("it requires exactly 3 argument");
		}

		checker2.check(arg2->car);
	}

	typedef typename T0::arg_type arg0_type;
	typedef typename T1::arg_type arg1_type;
	typedef typename T2::arg_type arg2_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	arg1_type get1() {
		return checker1.convert(arg0->cdr.ptr->car);
	}

	arg2_type get2() {
		return checker2.convert(arg0->cdr.ptr->cdr.ptr->car);
	}

	pair_cell::cxr getRemains() {
		return arg0->cdr.ptr->cdr.ptr->cdr;
	}


private:
	pair_cell * const arg0;
	T0 checker0;
	T1 checker1;
	T2 checker2;
};

//------------------------------------------------------------------------
template<class T0>
class ConsMustBe1 {
public:
	ConsMustBe1(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_nil(arg0->cdr)) {
			throw std::runtime_error("it requires exactly 1 argument");
		}

		checker0.check(arg0->car);
	}

	typedef typename T0::arg_type arg0_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	void set0(arg0_type a0) {
		arg0->car = checker0.to_cxr(a0);
	}

	static pair_cell *alloc(PairCellAllocator allocator, arg0_type a0) {
		pair_cell *pcell;
		pcell = allocator.alloc();
		ConsMustBe1 wrapper(pcell);
		wrapper.set0(a0);

		return pcell;
	}

private:
	pair_cell * const arg0;
	T0 checker0;
};

//------------------------------------------------------------------------
template<class T0, class T1>
class ConsMustBe2 {
public:
	ConsMustBe2(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_pair_cell_pointer(arg0->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker0.check(arg0->car);

		pair_cell *arg1 = arg0->cdr;
		if (is_nil(arg1)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_nil(arg1->cdr)) {
			throw std::runtime_error("it requires exactly 2 argument");
		}

		checker1.check(arg1->car);
	}

	typedef typename T0::arg_type arg0_type;
	typedef typename T1::arg_type arg1_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	arg1_type get1() {
		return checker1.convert(arg0->cdr.ptr->car);
	}

	void set0(arg0_type a0) {
		arg0->car = checker0.to_cxr(a0);
	}

	void set1(arg1_type a1) {
		arg0->cdr.ptr->car = checker1.to_cxr(a1);
	}

	static pair_cell *alloc(PairCellAllocator allocator, arg0_type a0, arg1_type a1) {
		pair_cell *pcell;
		pcell = allocator.alloc();
		pcell->cdr.ptr = allocator.alloc();
		ConsMustBe2 wrapper(pcell);
		wrapper.set0(a0);
		wrapper.set1(a1);

		return pcell;
	}

private:
	pair_cell * const arg0;
	T0 checker0;
	T1 checker1;
};

//------------------------------------------------------------------------
template<class T0, class T1, class T2>
class ConsMustBe3 {
public:
	ConsMustBe3(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_pair_cell_pointer(arg0->cdr)) {
			throw std::runtime_error("must be cons");
		}
		checker0.check(arg0->car);

		pair_cell *arg1 = arg0->cdr;
		if (is_nil(arg1)) {
			throw std::runtime_error("it requires exactly 3 argument");
		}
		if (!is_pair_cell_pointer(arg1->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker1.check(arg1->car);

		pair_cell *arg2 = arg1->cdr;
		if (is_nil(arg2)) {
			throw std::runtime_error("it requires exactly 3 argument");
		}
		if (!is_nil(arg2->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker2.check(arg2->car);
	}

	typedef typename T0::arg_type arg0_type;
	typedef typename T1::arg_type arg1_type;
	typedef typename T2::arg_type arg2_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	arg1_type get1() {
		return checker1.convert(arg0->cdr.ptr->car);
	}

	arg2_type get2() {
		return checker2.convert(arg0->cdr.ptr->cdr.ptr->car);
	}

	void set0(arg0_type a0) {
		arg0->car = checker0.to_cxr(a0);
	}

	void set1(arg1_type a1) {
		arg0->cdr.ptr->car = checker1.to_cxr(a1);
	}

	void set2(arg2_type a2) {
		arg0->cdr.ptr->cdr.ptr->car = checker2.to_cxr(a2);
	}

	static pair_cell *alloc(PairCellAllocator allocator, arg0_type a0, arg1_type a1, arg2_type a2) {
		pair_cell *pcell, *next0, *next1;

		pcell = allocator.alloc();
		next0 = allocator.alloc();
		next1 = allocator.alloc();

		pcell->car = 0;
		pcell->cdr.ptr = next0;

		next0->car = 0;
		next0->cdr.ptr = next1;

		next1->car = 0;
		next1->cdr = 0;

		ConsMustBe3<T0, T1, T2> wrapper(pcell);

		wrapper.set0(a0);
		wrapper.set1(a1);
		wrapper.set2(a2);

		return pcell;
	}

private:
	pair_cell * const arg0;
	T0 checker0;
	T1 checker1;
	T2 checker2;
};

//------------------------------------------------------------------------
template<class T0, class T1, class T2, class T3>
class ConsMustBe4 {
public:
	ConsMustBe4(pair_cell *arg0) : arg0(arg0) {}

	void check() {
		if (is_nil(arg0)) {
			throw std::runtime_error("must be cons");
		}
		if (!is_pair_cell_pointer(arg0->cdr)) {
			throw std::runtime_error("must be cons");
		}
		checker0.check(arg0->car);

		pair_cell *arg1 = arg0->cdr;
		if (is_nil(arg1)) {
			throw std::runtime_error("it requires exactly 4 argument");
		}
		if (!is_pair_cell_pointer(arg1->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker1.check(arg1->car);

		pair_cell *arg2 = arg1->cdr;
		if (is_nil(arg2)) {
			throw std::runtime_error("it requires exactly 4 argument");
		}
		if (!is_pair_cell_pointer(arg2->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker2.check(arg2->car);

		pair_cell *arg3 = arg2->cdr;
		if (is_nil(arg3)) {
			throw std::runtime_error("it requires exactly 4 argument");
		}
		if (!is_nil(arg3->cdr)) {
			throw std::runtime_error("must be cons");
		}

		checker3.check(arg3->car);
	}

	typedef typename T0::arg_type arg0_type;
	typedef typename T1::arg_type arg1_type;
	typedef typename T2::arg_type arg2_type;
	typedef typename T3::arg_type arg3_type;

	arg0_type get0() {
		return checker0.convert(arg0->car);
	}

	arg1_type get1() {
		return checker1.convert(arg0->cdr.ptr->car);
	}

	arg2_type get2() {
		return checker2.convert(arg0->cdr.ptr->cdr.ptr->car);
	}

	arg3_type get3() {
		return checker3.convert(arg0->cdr.ptr->cdr.ptr->cdr.ptr->car);
	}

	void set0(arg0_type a0) {
		arg0->car = checker0.to_cxr(a0);
	}

	void set1(arg1_type a1) {
		arg0->cdr.ptr->car = checker1.to_cxr(a1);
	}

	void set2(arg2_type a2) {
		arg0->cdr.ptr->cdr.ptr->car = checker2.to_cxr(a2);
	}

	void set3(arg3_type a3) {
		arg0->cdr.ptr->cdr.ptr->cdr.ptr->car = checker3.to_cxr(a3);
	}

	static pair_cell *alloc(PairCellAllocator allocator, arg0_type a0, arg1_type a1, arg2_type a2, arg3_type a3) {
		pair_cell *pcell, *next0, *next1, *next2;

		pcell = allocator.alloc();
		next0 = allocator.alloc();
		next1 = allocator.alloc();
		next2 = allocator.alloc();

		pcell->car = 0;
		pcell->cdr.ptr = next0;

		next0->car = 0;
		next0->cdr.ptr = next1;

		next1->car = 0;
		next1->cdr.ptr = next2;

		next2->car = 0;
		next2->cdr = 0;

		ConsMustBe4 wrapper(pcell);

		wrapper.set0(a0);
		wrapper.set1(a1);
		wrapper.set2(a2);
		wrapper.set3(a2);

		return pcell;
	}

private:
	pair_cell * const arg0;
	T0 checker0;
	T1 checker1;
	T2 checker2;
	T3 checker3;
};

//------------------------------------------------------------------------

class IsPairCellPointer {
public:
	void check(pair_cell::cxr u) {
		if (!is_pair_cell_pointer(u)) {
			throw std::runtime_error("arg must be pair_cell pointer.");
		}
	}

	typedef pair_cell *arg_type;

	pair_cell *convert(pair_cell::cxr u) {
		return static_cast<pair_cell *>(u);
	}

	pair_cell::cxr to_cxr(pair_cell *arg) {
		pair_cell::cxr rv;
		SIGN_ASSERT((arg->car.value & TAG_MASK) == TAG_POINTER);
		rv = arg;
		return rv;
	}
};

//----------------------------------------------------------------
class IsExAllocator {
public:
	void check(pair_cell::cxr u) {
		if (!is_ex_atom_entity_pointer(u)) {
			throw std::runtime_error("arg must be ex atom pointer.");
		}
	}

	typedef ExAllocator *arg_type;

	ExAllocator *convert(pair_cell::cxr u) {
		return static_cast<ExAllocator *>(u);
	}

	pair_cell::cxr to_cxr(ExAllocator *arg) {
		pair_cell::cxr rv;
		rv.ex_atom = arg;
		return rv;
	}
};

//----------------------------------------------------------------
template<class T>
class IsExAtom {
public:
	void check(pair_cell::cxr u) {
		if (!is_ex_atom_entity_pointer(u)) {
			throw std::runtime_error("arg must be ex atom pointer.");
		}
		T *ex_atom;
		ex_atom = static_cast<T *>(u);
		if ( ex_atom == 0 ) {
			throw std::runtime_error("arg must be specified ex atom.");
		}
	}

	typedef T *arg_type;

	T *convert(pair_cell::cxr u) {
		return static_cast<T *>(u);
	}

	pair_cell::cxr to_cxr(T *arg) {
		pair_cell::cxr rv;
		rv.ex_atom = arg;
		return rv;
	}
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_CONS_CHECKER_H__ */
