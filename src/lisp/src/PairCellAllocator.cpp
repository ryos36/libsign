/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#include <cstring>
#include "PairCellAllocator.h"
#include "pair_cell.h"
#include "ExSymbol.h"
#include "ExRoot.h"

#include "sign/sign_assert.h"
#include "exception.h"
#include <cstdlib>
#include <iostream>

using namespace sign::scheme;

#include "gc_debug.h"

PairCellAllocator::PairCellAllocator(int a_pcell_n, int a_align) :
	orignal_pcell_n(a_pcell_n), align(a_align),
	pcell_n(a_pcell_n), alloc_size(0), mem(0),
	top_of_pcell(0), end_of_pcell(0), free_pcell(0),
	free_n(-1), water_polcy(a_pcell_n), root(0),
	also_magic(MAGIC_NO)
{
}

//----------------------------------------------------------------
void
PairCellAllocator::malloc_align(int a_pcell_n, int a_align, void *&ind_mem, int &ind_alloc_size)
{
	int margin_for_magic;
	margin_for_magic = a_align * 2;

	ind_alloc_size = a_pcell_n * sizeof(pair_cell) + a_align + margin_for_magic;
	ind_mem = malloc(ind_alloc_size);

	return;
}

//----------------------------------------------------------------
void
PairCellAllocator::init_pcell(void *a_mem, int a_alloc_size, int a_pcell_n, int a_align, pair_cell *&ind_top_of_pcell, pair_cell *&ind_end_of_pcell)
{
	long mem_int;
	int align_mask;
	pair_cell *pcp;

	SIGN_ASSERT(sizeof(mem_int) == sizeof(void *));

	mem_int = reinterpret_cast<long>(a_mem);
	align_mask = a_align - 1;
	mem_int += (a_align - 1);
	mem_int &= ~align_mask;

	pcp = reinterpret_cast<pair_cell *>(mem_int);
	set_magic(pcp, sizeof(pair_cell));
	++pcp;

	ind_top_of_pcell = pcp;
	ind_end_of_pcell = &pcp[a_pcell_n];
	pcp = ind_end_of_pcell;
	set_magic(pcp, sizeof(pair_cell));

	ASSERT((void *)(pcp + 1) <= (void *)(((char *)a_mem) + a_alloc_size));
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::init_free_pcell(pair_cell *ind_top_of_pcell, pair_cell *ind_end_of_pcell, int &free_n)
{
	free_n = ind_end_of_pcell - ind_top_of_pcell;
	ind_top_of_pcell->car.ptr = 0;
	ind_top_of_pcell->cdr.value = free_n - 1;
	
	return ind_top_of_pcell;
#if 0
	pair_cell *pcp;
	for( pcp = ind_top_of_pcell; pcp != ind_end_of_pcell; ++pcp ) {
		pcp->car.ptr = (pcp + 1);
	}
	pcp = ind_end_of_pcell - 1;
	pcp->car.ptr = 0;
	free_n = ind_end_of_pcell - ind_top_of_pcell;

	return ind_top_of_pcell;
#endif
}

//----------------------------------------------------------------
void
PairCellAllocator::set_magic(void *area, int size)
{
	uint32_t *i32p;
	i32p = reinterpret_cast<uint32_t *>(area);
	for ( unsigned int i = 0; i < size/sizeof(uint32_t); ++i ) {
		*i32p = MAGIC_NO;
		++i32p;
	}
}

//----------------------------------------------------------------
void
PairCellAllocator::check_magic()
{
	SIGN_ASSERT(also_magic == MAGIC_NO);

	pair_cell *pcp;
	uint32_t *i32p;
	pcp = top_of_pcell;
	--pcp;
	i32p = (uint32_t *)pcp;
	SIGN_ASSERT( *i32p == MAGIC_NO );
	SIGN_ASSERT( *(i32p+1) == MAGIC_NO );

	pcp = end_of_pcell;
	i32p = (uint32_t *)pcp;
	SIGN_ASSERT( *i32p == MAGIC_NO );
	SIGN_ASSERT( *(i32p+1) == MAGIC_NO );
}

//----------------------------------------------------------------
bool
PairCellAllocator::init()
{
	malloc_align(pcell_n, align, mem, alloc_size);
	if ( mem == 0 ) {
		return false;
	}
	init_pcell(mem, alloc_size, pcell_n, align, top_of_pcell, end_of_pcell);
	check_magic();

	free_pcell = init_free_pcell(top_of_pcell, end_of_pcell, free_n);

	water_polcy.init_water();

	init_root();

	return true;
}

//----------------------------------------------------------------
bool
PairCellAllocator::re_init(int factor_n)
{
	int new_pcell_n;
	int new_alloc_size;
	void *new_mem;

	pair_cell *new_top_of_pcell;
	pair_cell *new_end_of_pcell;
	pair_cell *new_free_pcell;
	pair_cell *top_of_free_pcell;

	check_magic();
	new_pcell_n = pcell_n * factor_n;

	locker lock_it(this);

	malloc_align(new_pcell_n, align, new_mem, new_alloc_size);
	if ( new_mem == 0 ) {
		return false;
	}

	init_pcell(new_mem, new_alloc_size, new_pcell_n, align, new_top_of_pcell, new_end_of_pcell);

	top_of_free_pcell = copy_with_gc(new_top_of_pcell, new_end_of_pcell, top_of_pcell, end_of_pcell);
	new_free_pcell = init_free_pcell(top_of_free_pcell, new_end_of_pcell, free_n);

	::free(mem);

	mem = new_mem;
	alloc_size = new_alloc_size;
	pcell_n = new_pcell_n;
	
	top_of_pcell = new_top_of_pcell;
	end_of_pcell = new_end_of_pcell;
	free_pcell = new_free_pcell;

	return true;
}

//----------------------------------------------------------------
bool
PairCellAllocator::try_gc()
{
	int factor_n;
	bool ok;
	factor_n = water_polcy.eval_factor(pcell_n - free_n);
	if ( factor_n == 0 ) {
		return false;
	}
	ok = re_init(factor_n);

	SIGN_ASSERT(ok);
	water_polcy.update_low_water_after_gc(factor_n, pcell_n - free_n);

	return true;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::copy_with_gc(pair_cell *dst_top_of_pcell, pair_cell *dst_end_of_pcell, pair_cell *src_top_of_pcell, pair_cell *src_end_of_pcell)
{
	pair_cell *new_top_of_free, *scan_pointer;
	new_top_of_free = dst_top_of_pcell;

	// top must be ExAtom of ROOT
	SIGN_ASSERT(src_top_of_pcell->car.value == ROOT);

	*dst_top_of_pcell = *src_top_of_pcell;
	marked(src_top_of_pcell, dst_top_of_pcell);
	++new_top_of_free;

	scan_pointer = dst_top_of_pcell;

	do {
		view_gc(dst_top_of_pcell, new_top_of_free, src_top_of_pcell, src_top_of_pcell + 20);
		scan(scan_pointer, new_top_of_free);
	} while ( scan_pointer != new_top_of_free );

	view_gc(dst_top_of_pcell, new_top_of_free, src_top_of_pcell, src_top_of_pcell + 10);

	/* old root is moved. so marked and pointer */
	SIGN_ASSERT((root->car.value == 0x2) &&
	            ((root->cdr.value & TAG_MASK) == 0));
	root = static_cast<pair_cell *>(root->cdr);

	return new_top_of_free;
}

//----------------------------------------------------------------
namespace {
inline pair_cell * 
move_and_mark(pair_cell *&new_top_of_free, pair_cell *src_pcell) 
{
	pair_cell *rv;
	rv = new_top_of_free;
	*new_top_of_free = *src_pcell;
	int copy_n;

	copy_n = 0;
	if ( is_ex_atom_entity(src_pcell)) {
		int copy_size;
		if ( has_continus_block(src_pcell)) {
			copy_size = src_pcell->cdr.value;
			pair_cell *target_pcell;
			target_pcell = src_pcell + 1;

			memcpy(new_top_of_free + 1, target_pcell, copy_size);
			copy_n = (copy_size + sizeof(pair_cell) - 1)/sizeof(pair_cell);
#ifdef DEBUG
			for( int i = 0 ; i < copy_n; ++i ) {
				target_pcell->car.value = MARKED_FLAG;
				target_pcell->cdr.value = 0xFFFFFFFF;
				++target_pcell;
			}
#endif
		}
	}

	marked(src_pcell, new_top_of_free);
	++new_top_of_free;
	new_top_of_free += copy_n;

	return rv;
}

/*
inline void
try_copy_continuous_block(pair_cell *&new_top_of_free, pair_cell *target_pcell) 
{
	pair_cell *p;
	p = target_pcell->cdr.ptr; // since already moved.
	if ( is_ex_atom_entity(p)) {
		int copy_size;
		if ( has_continus_block(p)) {
			copy_size = p->cdr.value;

			++target_pcell;
			memcpy(new_top_of_free, target_pcell, copy_size);
			int copy_n;
			copy_n = (copy_size + sizeof(pair_cell) - 1)/sizeof(pair_cell);
			new_top_of_free += copy_n;
#ifdef DEBUG
			for( int i = 0 ; i < copy_n; ++i ) {
				target_pcell->car.value = MARKED_FLAG;
				target_pcell->cdr.value = 0xFFFFFFFF;
				++target_pcell;
			}
#endif
		}
	}
}
*/

} // namespace

//----------------------------------------------------------------
void
PairCellAllocator::scan(pair_cell *&scan_pointer, pair_cell *&new_top_of_free)
{
	pair_cell *pcell;
	int skip_n = 0;
	if ( is_ex_atom_entity(scan_pointer)) {
		if ( has_successor_pair_cell(scan_pointer)) {
			pcell = move_and_mark(new_top_of_free, static_cast<pair_cell *>(scan_pointer->cdr));
			scan_pointer->cdr = pcell;
		} else if ( has_continus_block(scan_pointer)) {
			int skip_size;
			skip_size = scan_pointer->cdr.value;
			skip_n = (skip_size + sizeof(pair_cell) - 1)/sizeof(pair_cell);
		} else {
			// nothing to do
		}
	} else {
		pair_cell *target_pcell;

		if (need_move(scan_pointer->car)) {
			target_pcell = static_cast<pair_cell *>(scan_pointer->car);
			if (!is_marked_pair_cell(target_pcell)) {
				pcell = move_and_mark(new_top_of_free, target_pcell);
				scan_pointer->car = pcell;
			} else {
				scan_pointer->car = target_pcell->cdr;
			}
		}

		if (need_move(scan_pointer->cdr)) {
			target_pcell = static_cast<pair_cell *>(scan_pointer->cdr);
			if (!is_marked_pair_cell(target_pcell)) {
				pcell = move_and_mark(new_top_of_free, target_pcell);
				scan_pointer->cdr = pcell;
			} else {
				scan_pointer->cdr = target_pcell->cdr;
			}
		}
	}

	++scan_pointer;
	scan_pointer += skip_n;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc0()
{
	locker lock_it(this);

	pair_cell *pcp;

	pcp = free_pcell;
	if ( pcp == 0 ) {
		return 0;
	}

	if ( pcp->cdr.value == 0 ) {
		free_pcell = pcp->car.ptr;
	} else {
		free_pcell = (pcp + 1);
		free_pcell->car.ptr = pcp->car.ptr;
		free_pcell->cdr.value = pcp->cdr.value - 1;
	}
	--free_n;

	pcp->car.value = 0;
	pcp->cdr.value = 0;

	return pcp;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::continus_alloc0(int n)
{
	locker lock_it(this);

	pair_cell *pcp, **old_pcp;

	old_pcp = &free_pcell;
	pcp = free_pcell;

	do {
		if ( pcp == 0 ) {
			return 0;
		}
		if ( n <= (pcp->cdr.value + 1)) {
			break;
		} 
		old_pcp = &pcp->car.ptr;
		pcp = pcp->car.ptr;
	} while ( true );
	
	if ( pcp->cdr.value == (n - 1) ) {
		*old_pcp = pcp->car.ptr;
	} else {
		*old_pcp = (pcp + n);
		(*old_pcp)->car.ptr = pcp->car.ptr;
		(*old_pcp)->cdr.value = pcp->cdr.value - n;
	}

	free_n -= n;

	pcp->car.value = 0;
	pcp->cdr.value = 0;

	return pcp;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc_value(ExAtomType type, long int value)
{
	pair_cell *pcell;

	pcell = alloc();
	pcell->car.value = type;
	pcell->cdr.value = value;

	return pcell;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc(ExAtomType type, void *ex_atom)
{
	pair_cell *pcell;

	pcell = alloc();
	pcell->car.value = type;
	pcell->cdr.ex_atom = ex_atom;

	return pcell;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc(ExAtomType type, const void *ex_mem, int size)
{
	int alloc_n;
	alloc_n = (size + sizeof(pair_cell) * 2 - 1)/sizeof(pair_cell);

	pair_cell *pcell;

	pcell = continus_alloc0(alloc_n);
	if ( pcell == 0 ) {
		throw alloc_error("continus alloc: no space.");
	}
	ASSERT(( type & HAS_CONTINUOUS_N_FLAG ) == HAS_CONTINUOUS_N_FLAG);
	pcell->car.value = type;

	pcell->cdr.value = (size << LENGTH_SHIFT) | pair_cell::CONTINOUS_BLOCK;
	
	memcpy((pcell + 1), ex_mem, size);

	return pcell;
}

//----------------------------------------------------------------
void
PairCellAllocator::free_pair_cell(pair_cell *p)
{
	locker lock_it(this);

	p->car.ptr = free_pcell;
	p->cdr.value = 0;
	free_pcell = p;
	++free_n;
}

//----------------------------------------------------------------
void
PairCellAllocator::init_root()
{
	pair_cell *env;
	root = alloc_root();
	env = alloc_environment(0);
	set_env(env);
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc_root()
{
	pair_cell *ex_root, *symbol_list, *env;

	ex_root = alloc();
	symbol_list = alloc();
	env = alloc();

	ex_root->car.value = ROOT;
	ex_root->cdr = symbol_list;

	symbol_list->car = 0;
	symbol_list->cdr = env;;

	env->car = 0;
	env->cdr = 0;;

	return ex_root;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc_environment(pair_cell *mama_env_ptr)
{
	pair_cell *env, *env_entity, *pcell, *values, *funcs;

	env = alloc();
	env_entity = alloc(ALLOCATOR, this);
	pcell = alloc();
	values = alloc();	
	funcs = alloc();

	env->car = env_entity;
	env->cdr = pcell;

	pcell->car = mama_env_ptr;
	pcell->cdr = values;

	values->car = 0; // key, value pointer
	values->cdr = funcs; 

	funcs->car = 0; // fun_name, value pointer
	funcs->cdr = 0;

	return env;
}

//----------------------------------------------------------------
void
PairCellAllocator::set_env(pair_cell *env)
{
	SIGN_ASSERT(is_pair_cell_pointer(root->cdr));
	SIGN_ASSERT(is_pair_cell_pointer(root->cdr.ptr->cdr));
	root->cdr.ptr->cdr.ptr->car = env;
}


//----------------------------------------------------------------

PairCellAllocator::~PairCellAllocator()
{
	if ( mem ) {
		::free( mem );
		mem = 0;
	}
}

//----------------------------------------------------------------
int
PairCellAllocator::calc_align_shift(int align)
{
	int align_mask;
	align_mask = align - 1;
	int i;
	for( i = 0 ; i < 32; ++i ) {
		if (( align_mask & 1 ) == 0 ) {
			break;
		}
		align_mask >>= 1;
	}
	SIGN_ASSERT(i != 32);
	return i;
}

//----------------------------------------------------------------
ExSymbol *
PairCellAllocator::find_symbol(PairCellAllocator &allocator, const char *c_symbol)
{
	ExRoot *root;
	root = reinterpret_cast<ExRoot *>(allocator.root);

	ExSymbol *symbol;
	symbol = root->search(c_symbol);
	return symbol;
}

//----------------------------------------------------------------
ExSymbol *
PairCellAllocator::register_symbol(PairCellAllocator &allocator, const char *c_symbol)
{
	ExRoot *root;
	root = reinterpret_cast<ExRoot *>(allocator.root);

	ExSymbol *symbol;
	symbol = root->search(c_symbol);
	if ( symbol == 0 ) {
		symbol = root->register_symbol(allocator, c_symbol);
	}
	SIGN_ASSERT(symbol);
	return symbol;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc_set2(PairCellAllocator &allocator, void *ptr0, void *ptr1)
{
	pair_cell *pcell0, *next1;

	pcell0 = allocator.alloc();
	pcell0->car.ex_atom = ptr0;
	pcell0->cdr.ex_atom = next1 = allocator.alloc();

	next1->car.ex_atom = ptr1;
	SIGN_ASSERT(is_nil(next1->cdr));

	return pcell0;
}

//----------------------------------------------------------------
pair_cell *
PairCellAllocator::alloc_set3(PairCellAllocator &allocator, void *ptr0, void *ptr1, void *ptr2)
{
	pair_cell *pcell0, *next1, *next2;

	pcell0 = allocator.alloc();
	pcell0->car.ex_atom = ptr0;
	pcell0->cdr.ex_atom = next1 = allocator.alloc();

	next1->car.ex_atom = ptr1;
	next1->cdr.ex_atom = next2 = allocator.alloc();

	next2->car.ex_atom = ptr2;
	SIGN_ASSERT(is_nil(next2->cdr));

	return pcell0;
}

//----------------------------------------------------------------
