/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_SREADER_H__
#define __SIGN_SCHEME_SREADER_H__

#include <ostream>
#include <stdexcept>
#include "sign/RCPointer.h"
#include "pair_cell.h"

namespace sign {
namespace scheme {

class PairCellAllocator;

namespace reader {

class Token;
class TokenReader;

class SReader {
public:
	SReader(PairCellAllocator &an_allocator);

	void init();
	pair_cell::cxr read(TokenReader* reader);

#define __MAKE_STATUS__(__v__) (((__v__) << TAG_SHIFT) | ILLEGAL_TAG)
	enum {
		TARGET_STATUS_CAR_FREE = __MAKE_STATUS__(0),
		TARGET_STATUS_CAR_OCCUPIED = __MAKE_STATUS__(1),
		TARGET_STATUS_CAR_OCCUPIED_DOT_CDR = __MAKE_STATUS__(2),
		TARGET_STATUS_BOTH_ARE_NULL = __MAKE_STATUS__(3),
		TARGET_STATUS_BOTH_ARE_OCCUPIED_DOT_CDR = __MAKE_STATUS__(4),
	};
#undef __MAKE_STATUS__

private:
        enum Status {
                INIT,
                WAIT_CAR,
                WAIT_NEXT_CELL,
                QUOTE_START,
                WAIT_CAR_AFTER_QUOTE_START,
                AFTER_DOT,
                SKIP_RIGHT_PAREN,

                FINISHED,

                ERROR = -2,
		NOT_DEFINED = -1,
        };


	PairCellAllocator &allocator;
	pair_cell::cxr root_cxr;
	pair_cell::cxr *back_pointer;

	void procedure(RCPointer<Token> token);

	int get_target_status(pair_cell *target);
	void set_target_status(pair_cell *target, int target_status);

	pair_cell::cxr make_atom(RCPointer<Token> token);
	void set_cxr(pair_cell::cxr a_cxr);

	void insert_target(pair_cell *new_target);
	void pop_target();
	void init_root_cxr();
	void free_almost_pair_cell();

	void add_pair_cell();
	void add_pair_cell_quote_on_top(RCPointer<Token> token);
	void close_list();
	void detect_dot();

	void syntax_error(const char *c_msg, RCPointer<Token> token = 0) throw(std::runtime_error&);
	void fatal_error(const char *c_msg, RCPointer<Token> token = 0) throw(std::runtime_error&);
	/*
	void fatal_error(const char *c_msg) throw(std::runtime_error&) {
		throw std::runtime_error(c_msg);
	}
	*/

	bool is_car_free(unsigned long value) {
		return ( value == SReader::TARGET_STATUS_CAR_FREE) || 
			( value == SReader::TARGET_STATUS_BOTH_ARE_NULL);
	}

public:
	void view(std::ostream &out, const std::string &separate_line); // for debug
	void view(std::ostream &out); // for debug

	static const std::string default_separate_line;
};

} // namespace reader
} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_SREADER_H__ */
