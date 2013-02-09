#ifndef __SIGN_SCHEME_CELL_ALLOCATOR_H__
#define __SIGN_SCHEME_CELL_ALLOCATOR_H__

#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/gc.h"

namespace sign {
namespace scheme {

class Cell;
class ExAtom;

class CellAllocator {
public:
	class SystemInterface {
	public:
		SystemInterface() : successor(0) {}
		SystemInterface *successor;

		virtual void suspend() = 0;	
		virtual void resume() = 0;	
	};
public:
	CellAllocator(int cell_n);
	~CellAllocator();

	Cell *alloc(int n);
	ExAtom *alloc(int n, ExAtom::Type type, void *ptr);

	void gc();
	int max_cell_n() const;
	int using_cell_n() const;

#ifdef DEBUG
public:
	static void checkRootEnvironment();
#endif
private:
	int cell_n;

	Cell *top, *end, *top_of_free;
	void *memptr;

	void *old_memptr;

	Cell *low_water;

	SystemInterface *systemInterface;

	CellAllocator *next_pool;

	void scan(CellAllocator &new_allocator, Cell *&);
	void marked(Cell *marked_cell, Cell *cell_ptr);
	void marked(ExAtom *marked_cell, ExAtom *ex_atom_ptr);
	Cell *copyCellTo(Cell *, CellAllocator &new_allocator);
	Union copyExAtomTo(ExAtom *, CellAllocator &new_allocator);
	Union copyUnionTo(Union , CellAllocator &new_allocator);

	bool needMove(Union u) const;

	void free_with_ExAtom();
#ifdef DEBUG
	static void view_gc(CellAllocator &ca, const char *msg = 0);
#else
	static void view_gc(CellAllocator &ca, const char *msg = 0) {};
#endif
public:
	static CellAllocator &getCellAllocator();
private:
	friend class Cell;
	friend class ExAtom;
	friend int gc::current_max_cell_n();
	friend int gc::current_using_cell_n();
public:
	static void do_gc();
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_CELL_ALLOCATOR_H__ */
