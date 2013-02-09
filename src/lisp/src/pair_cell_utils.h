#ifndef __SIGN_SCHEME_PAIR_CELL_UTILS_H__
#define __SIGN_SCHEME_PAIR_CELL_UTILS_H__

//----------------------------------------------------------------
inline bool
is_nil(pair_cell *pcell)
{
	return (pcell == 0);
}

//----------------------------------------------------------------
inline bool
is_ex_atom_entity(pair_cell *pcell)
{
	if ( is_nil(pcell) ) {
		return false;
	}
	return (pcell->car.value & TAG_MASK) == EX_ATOM_ENTITY;
}

//----------------------------------------------------------------
inline bool is_illegal_tag(pair_cell *pcell)
{
	// NOTE: check only CDR
	return (pcell->cdr.value & TAG_MASK) == ILLEGAL_TAG;
}

//----------------------------------------------------------------
inline bool
is_nil(pair_cell::cxr cxr)
{
	return (cxr.value == 0);
}

//----------------------------------------------------------------
inline bool
is_short_num(pair_cell::cxr cxr)
{
	return (cxr.value & TAG_MASK) == ATOM_SHORT_NUM ;
}

//----------------------------------------------------------------
inline bool is_pointer(pair_cell::cxr cxr)
{
	if ( is_nil(cxr) ) {
		return true;
	}
	return ((cxr.value & TAG_MASK) == TAG_POINTER ) ;
}

//----------------------------------------------------------------
inline bool
is_list(pair_cell::cxr cxr)
{
	if ( is_nil(cxr) ) {
		return false;
	}

	if ((cxr.value & TAG_MASK) != TAG_POINTER ) {
		return false;
	}
	return (( cxr->car.value & TAG_MASK ) != EX_ATOM_ENTITY );
}

//----------------------------------------------------------------
inline bool
is_pair_cell_pointer(pair_cell::cxr cxr)
{
	if ( is_nil(cxr) ) {
		return true;
	}

	if ((cxr.value & TAG_MASK) != TAG_POINTER ) {
		return false;
	}
	return (( cxr->car.value & TAG_MASK ) != EX_ATOM_ENTITY );
}

//----------------------------------------------------------------
inline bool
is_ex_atom_entity_pointer(pair_cell::cxr cxr) {
	if ( is_nil(cxr) ) {
		return false;
	}
	if ((cxr.value & TAG_MASK) != TAG_POINTER ) {
		return false;
	}
	return (( cxr->car.value & TAG_MASK ) == EX_ATOM_ENTITY );
}

//----------------------------------------------------------------
inline bool
is_atom(pair_cell::cxr cxr)
{
	if ( is_nil(cxr) ) {
		// nil is atom
		return true;
	}

	int tag;
	tag = cxr.value & TAG_MASK;
	if ( tag & ATOM_FLAG ) {
		return true;
	}

	SIGN_ASSERT(tag == 0); // because ( pair_cell * ) or (ex_atom pointer)
	return ((cxr->car.value & TAG_MASK) == EX_ATOM_ENTITY);
}

//----------------------------------------------------------------
inline ExAtomType
get_ex_atom_type(pair_cell *p)
{
	return (ExAtomType)p->car.value;
}

//----------------------------------------------------------------

#endif /* __SIGN_SCHEME_PAIR_CELL_UTILS_H__ */
