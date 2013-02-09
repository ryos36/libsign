/* Copyright (C) 2008, 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_SCHEME_STATIC_SYMBOL_H__
#define __SIGN_SCHEME_STATIC_SYMBOL_H__

#include <string>

namespace sign {
namespace scheme {

extern const std::string QUOTE;
extern const std::string QUASIQUOTE;
extern const std::string UNQUOTE;
extern const std::string UNQUOTE_SPLICING;
extern const std::string SET_FORCE;
extern const std::string DEFINE;
extern const std::string IF;
extern const std::string LAMBDA;
extern const std::string BEGIN;
extern const std::string COND;
extern const std::string COND_ELSE;
extern const std::string COND_ARROW;
extern const std::string LET;

} // namespace scheme
} // namespace sign

#endif  /* __SIGN_SCHEME_STATIC_SYMBOL_H__ */
