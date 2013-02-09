/* Copyright (C) 2013 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_EXCEPTION_H__
#define __SIGN_SCHEME_EXCEPTION_H__

#include <stdexcept>
#include <string>

namespace sign {
namespace scheme {

class fatal_error : public std::logic_error {
public:
	explicit fatal_error(const std::string &__arg) : logic_error(__arg){};
};

class alloc_error : public std::logic_error {
public:
	explicit alloc_error(const std::string &__arg) : logic_error(__arg){};
};

class syntax_error : public std::logic_error {
public:
	explicit syntax_error(const std::string &__arg) : logic_error(__arg){};
};

class runtime_error : public std::logic_error {
public:
	explicit runtime_error(const std::string &__arg) : logic_error(__arg){};
};

// eor = end of reading
class eor_exception : public std::exception {
public:
	explicit eor_exception() {};
};

} // scheme
} // sign

#endif /* __SIGN_SCHEME_EXCEPTION_H__ */
