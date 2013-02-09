#ifndef __SIGN_SCHEME_GC_H__
#define __SIGN_SCHEME_GC_H__

namespace sign {
namespace scheme {
namespace gc {
	extern int alloc_n;
	extern int gc_n;

	extern int ex_atom_n;

	extern int ex_atom_type_n[];

	extern bool is_need_gc;

	int current_max_cell_n();
	int current_using_cell_n();
	int current_freed_cell_n();
} // namespace gc
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_GC_H__ */
