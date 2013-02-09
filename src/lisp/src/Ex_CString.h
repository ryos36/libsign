/* Copyright (C) 2012-2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EX_C_STRING_H__
#define __SIGN_SCHEME_EX_C_STRING_H__

#include <stdint.h>
#include <cstring>
#include <algorithm>
#include <string>
#include "pair_cell.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

class Ex_CString {
protected:
	Ex_CString(); 

public:
	union {
		struct {
			ExAtomType tag;
			union {
				unsigned long length_shift4;
				const char *c_str;
			} length_or_pointer;
		} c_string_header;
		pair_cell _pair_cell;
	} header;

	ExAtomType get_tag() {
		return header.c_string_header.tag;
	}

	const char *get_c_str() const {
		if (header.c_string_header.length_or_pointer.length_shift4 & 1){
			return reinterpret_cast<const char *>((this + 1));
		} else {
			return header.c_string_header.length_or_pointer.c_str;
		}
	}

	explicit operator const char *() {
		return get_c_str();
	}

	bool operator < (const Ex_CString &sym) {
		int v;
		v = cmp(sym);
		return ( v < 0 );
	}

	bool operator == (const Ex_CString &sym) {
		int v;
		v = cmp(sym);
		return ( v == 0 );
	}

	bool operator < (const char *c_str) {
		int v;
		v = cmp(c_str);
		return ( v < 0 );
	}

	bool operator == (const char *c_str) {
		int v;
		v = cmp(c_str);
		return ( v == 0 );
	}

	
private:
	// const char symbol_c_str[1];

	int cmp(const Ex_CString &sym) {
		return ::strcmp(get_c_str(), sym.get_c_str());
	}

	int cmp(const char *c_str) {
		std::string s(c_str);
		std::transform (s.begin (), s.end (), s.begin (), toupper);
		return ::strcmp(get_c_str(), c_str);
	}
	
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_EX_C_STRING_H__ */
