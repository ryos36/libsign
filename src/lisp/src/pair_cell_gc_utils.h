#ifndef __SIGN_SCHEME_PAIR_CELL_GC_UTILS_H__
#define __SIGN_SCHEME_PAIR_CELL_GC_UTILS_H__

//----------------------------------------------------------------
inline bool
need_move(const pair_cell::cxr cxr)
{
	if ( is_nil(cxr)) {
		return false;
	}
	if ( !is_pointer(cxr) ) {
		return false;
	}
	return true;
}

//----------------------------------------------------------------
inline bool
is_marked_pair_cell(const pair_cell *pcell) 
{
	return (pcell->car.value & MARKED_FLAG) == MARKED_FLAG;
}

//----------------------------------------------------------------
inline bool
has_successor_pair_cell(const pair_cell *pcell)
{
	return (( pcell->car.value & HAS_SUCCESSOR_PAIR_CELL_FLAG ) == HAS_SUCCESSOR_PAIR_CELL_FLAG);
}

//----------------------------------------------------------------
inline bool
has_continus_block(const pair_cell *pcell)
{
	return (pcell->car.value & HAS_CONTINUOUS_N_FLAG) == HAS_CONTINUOUS_N_FLAG;
}

//----------------------------------------------------------------
inline void
marked(pair_cell *pcell, pair_cell *new_ptr) {
	pcell->cdr.ptr = new_ptr;

	pcell->car.value = MARKED_FLAG;
}
//----------------------------------------------------------------

#endif /* __SIGN_SCHEME_PAIR_CELL_GC_UTILS_H__ */
