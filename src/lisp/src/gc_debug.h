/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __GC_DEBUG_H__
#define __GC_DEBUG_H__

#include <iostream>

namespace {

int view_one_pair_cell(pair_cell *pcell, bool car_in, bool cdr_in)
{
	int skip_n = 0;
	long tag;
	tag = pcell->car.value & TAG_MASK;
	char c = '?';
	if ( tag == (TAG_POINTER|MARKED_FLAG )) {
		c = 'm' ;
	} else {
		switch(tag) {
		case TAG_POINTER:
			if (pcell->car.value == 0) {
				c = '0';
			} else {
				if ( car_in ) {
					if ( is_pair_cell_pointer(pcell->car)) {
						c = 'P';
					} else {
						c = 'e';
					}

				} else {
					c = 'X';
				}
			}
			break;
		/*
		case EX_ATOM_ENTITY_POINTER:
			if ( car_in ) {
				c = 'e';
			} else {
				c = 'x';
			}
			break;
		*/
		case EX_ATOM_ENTITY:
			c = 'E';
			if ( pcell->car.value & HAS_CONTINUOUS_N_FLAG ) {
				skip_n = (pcell->cdr.value + sizeof(pair_cell) - 1)/sizeof(pair_cell);
				//std::cout << "<skip_n:" << skip_n << ">";
			}

			break;
		case ATOM_SHORT_NUM:
			c = 'N';
			break;
		}
	}
	std::cout << c ;

	if ( c == 'm' ) {
		c = '?';
		tag = pcell->cdr.value & TAG_MASK;

		if ( is_pair_cell_pointer(pcell->cdr)) {
			c = 'P';
		} else if (is_ex_atom_entity_pointer(pcell->cdr)) {
			c = 'e';
		} else {
			c = '?';
		}
		/*
		switch(tag) {
		case TAG_POINTER:
			c = 'P';
			break;
		case EX_ATOM_ENTITY_POINTER:
			c = 'e';
			break;
		default:
			c = 'm';
			break;
		}
		*/
	} else if ( tag != EX_ATOM_ENTITY ) {
		c = '?';
		tag = pcell->cdr.value & TAG_MASK;

		switch(tag) {
		case TAG_POINTER:
			if (pcell->cdr.value == 0) {
				c = '0';
			} else {
				if ( cdr_in ) {
					if ( is_pair_cell_pointer(pcell->car)) {
						c = 'P';
					} else {
						c = 'e';
					}
				} else {
					c = 'X';
				}
			}
			break;
		/*
		case EX_ATOM_ENTITY_POINTER:
			if ( cdr_in ) {
				c = 'e';
			} else {
				c = 'x';
			}
			break;
		*/
		case ATOM_SHORT_NUM:
			//std::cout << "<num:" << pcell->cdr.value << ">";
			c = 'N';
			break;
		}
	} else {
		c = ' ';
	}
	std::cout << c ;
	std::cout << ":";

	return skip_n;
}

void view_gc(pair_cell *dst_top_of_pcell, pair_cell *new_top_of_free, pair_cell *src_top_of_pcell, pair_cell *top_of_free_pcell)
{
	pair_cell *pcell;
	int skip_n;
	for( pcell = src_top_of_pcell; pcell < top_of_free_pcell; ++pcell) {
		skip_n = view_one_pair_cell(pcell,
			(src_top_of_pcell <= pcell->car.ptr) &&
			(pcell->car.ptr < top_of_free_pcell),
			(src_top_of_pcell <= pcell->cdr.ptr) &&
			(pcell->cdr.ptr < top_of_free_pcell));

		pcell += skip_n;
		if ( skip_n ) {
			for( int i = 0 ; i < skip_n; ++i ) {
				std::cout << "uu:";
			}
		}
	}
	std::cout << std::endl;
	
	for( pcell = dst_top_of_pcell; pcell < new_top_of_free; ++pcell) {
		skip_n = view_one_pair_cell(pcell,
			(dst_top_of_pcell <= pcell->car.ptr) &&
			(pcell->car.ptr < new_top_of_free),
			(dst_top_of_pcell <= pcell->cdr.ptr) &&
			(pcell->cdr.ptr < new_top_of_free));
		pcell += skip_n;
		if ( skip_n ) {
			for( int i = 0 ; i < skip_n; ++i ) {
				std::cout << "uu:";
			}
		}
	}
	std::cout << std::endl;
	std::cout << "---------------------" << std::endl;
	
}

} // namespace

#endif // __GC_DEBUG_H__
