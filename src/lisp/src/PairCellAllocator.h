#ifndef __SIGN_SCHEME_PAIR_CELL_ALLOCATOR_H__
#define __SIGN_SCHEME_PAIR_CELL_ALLOCATOR_H__

/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#include <stdexcept>
#include "pair_cell.h"
#include "WaterPolicy.h"

namespace sign {
namespace scheme {

class Environment;
class Scheme;
class ExSymbol;

class PairCellAllocator {
public:
	friend class Environment;
	friend class Scheme;
public:
	PairCellAllocator(int a_pcell_n, int a_align = (sizeof(void *) * 2));
	~PairCellAllocator();

	bool init();

private:
	pair_cell *alloc0();
	pair_cell *continus_alloc0(int n);
public:
	pair_cell *alloc();

	pair_cell *alloc_value(ExAtomType type, long int value);
	pair_cell *alloc(ExAtomType type, void *ex_atom);
	pair_cell *alloc(ExAtomType type, const void *ex_mem, int size);
	void free_pair_cell(pair_cell *);

	bool re_init(int factor_n = 1);
	bool try_gc();

#ifdef P_DEBUG
	pair_cell *get_root() {
		return root;
	}
#endif
private:
	const int orignal_pcell_n;
	const int align;
	int pcell_n;
	int alloc_size;
	void *mem;

	pair_cell *top_of_pcell;
	pair_cell *end_of_pcell;
	pair_cell *free_pcell;

	int free_n;
	WaterPolicy water_polcy;

	pair_cell *root;

	uint32_t also_magic; 

	enum {
		MAGIC_NO = 0xCAFEBABE
	};

	static void malloc_align(int a_pcell_n, int a_align, void *&ind_mem, int &ind_alloc_size);
	static void init_pcell(void *a_mem, int a_alloc_size, int a_pcell_n, int a_align, pair_cell *&ind_top_of_pcell, pair_cell *&ind_end_of_pcell);
	static void set_magic(void *area, int size);
	static pair_cell *init_free_pcell(pair_cell *ind_top_of_pcell, pair_cell *ind_end_of_pcell, int &free_n);

	void check_magic();

	template <class T> class _locker {
	public:
		_locker(T *_t) : t(_t) {
			t->lock();
		}
		~_locker() {
			t->unlock();
		}
		T *t;
	};

	class locker;
	typedef _locker<PairCellAllocator> locker;
	friend class locker;

	void lock() {}
	void unlock() {}

	pair_cell *copy_with_gc(pair_cell *dst_top_of_pcell, pair_cell *dst_end_of_pcell, pair_cell *src_top_of_pcell, pair_cell *src_end_of_pcell);
	void scan(pair_cell *&scan_pointer, pair_cell *&new_top_of_free);

	void init_root();
	pair_cell *alloc_root();
	pair_cell *alloc_environment(pair_cell *mama_env_ptr);
	void set_env(pair_cell *env);

	static int calc_align_shift(int align);
public:
	static ExSymbol* find_symbol(PairCellAllocator &allocator, const char *c_str);
	static ExSymbol* register_symbol(PairCellAllocator &allocator, const char *c_str);
	static pair_cell *alloc_set2(PairCellAllocator &allocator, void *ptr0, void *ptr1);
	static pair_cell *alloc_set3(PairCellAllocator &allocator, void *ptr0, void *ptr1, void *ptr2);
};

//----------------------------------------------------------------
inline
pair_cell *
PairCellAllocator::alloc()
{
	pair_cell *pcell;

	pcell = alloc0();

	if ( pcell == 0 ) {
		throw std::runtime_error("alloc0: no space.");
	}

	return pcell;
}
//----------------------------------------------------------------

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_PAIR_CELL_ALLOCATOR_H__ */
