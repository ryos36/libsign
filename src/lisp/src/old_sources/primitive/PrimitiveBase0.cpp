#include <stdexcept>
#include "sign/scheme++/Union.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/Environment.h"
#include "sign/scheme++/PrimitiveProcedure.h"
#include "sign/scheme++/RegisterPrimitive.h"

using namespace sign::scheme;

namespace sign {
namespace scheme {
namespace primitive {
	Union null_p(ExAtom *env, Cell *args);
	Union pair_p(ExAtom *env, Cell *args);
	Union number_p(ExAtom *env, Cell *args);
	Union integer_p(ExAtom *env, Cell *args);
	Union char_p(ExAtom *env, Cell *args);
	Union boolean_p(ExAtom *env, Cell *args);
	Union procedure_p(ExAtom *env, Cell *args);
	Union string_p(ExAtom *env, Cell *args);
	Union symbol_p(ExAtom *env, Cell *args);
	Union scheme_not(ExAtom *env, Cell *args);

	Union scheme_plus(ExAtom *env, Cell *args);
	Union scheme_minus(ExAtom *env, Cell *args);
	Union scheme_mul(ExAtom *env, Cell *args);
	Union scheme_div(ExAtom *env, Cell *args);
	Union scheme_modulo(ExAtom *env, Cell *args);
	Union scheme_remainder(ExAtom *env, Cell *args);

	Union eq_p(ExAtom *env, Cell *args);
	Union eqv_p(ExAtom *env, Cell *args);
	Union equal_p(ExAtom *env, Cell *args);

	Union scheme_less_than(ExAtom *env, Cell *args);
	Union scheme_less_equal(ExAtom *env, Cell *args);
	Union scheme_num_equal(ExAtom *env, Cell *args);
	Union scheme_greater_than(ExAtom *env, Cell *args);
	Union scheme_greater_equal(ExAtom *env, Cell *args);
	Union scheme_odd_p(ExAtom *env, Cell *args);
	Union scheme_even_p(ExAtom *env, Cell *args);
	Union scheme_negative_p(ExAtom *env, Cell *args);
	Union scheme_positive_p(ExAtom *env, Cell *args);

	Union memq(ExAtom *env, Cell *args);
	Union memv(ExAtom *env, Cell *args);
	Union member(ExAtom *env, Cell *args);
	Union assq(ExAtom *env, Cell *args);
	Union assv(ExAtom *env, Cell *args);
	Union assoc(ExAtom *env, Cell *args);

	Union scheme_abs(ExAtom *env, Cell *args);
	Union scheme_zero_p(ExAtom *env, Cell *args);
	Union scheme_max(ExAtom *env, Cell *args);
	Union scheme_min(ExAtom *env, Cell *args);

	Union cons(ExAtom *env, Cell *args);
	Union length(ExAtom *env, Cell *args);
	Union list(ExAtom *env, Cell *args);
	Union set_car(ExAtom *env, Cell *args);
	Union set_cdr(ExAtom *env, Cell *args);
	Union list_ref(ExAtom *env, Cell *args);
	Union list_p(ExAtom *env, Cell *args);
	Union append(ExAtom *env, Cell *args);
	Union reverse(ExAtom *env, Cell *args);

	Union char2integer(ExAtom *env, Cell *args);
	Union char_alphabetic_p(ExAtom *env, Cell *args);
	Union char_le(ExAtom *env, Cell *args);
	Union char_lt(ExAtom *env, Cell *args);
	Union char_eq(ExAtom *env, Cell *args);
	Union char_ge(ExAtom *env, Cell *args);
	Union char_gt(ExAtom *env, Cell *args);
	Union char_le_ci(ExAtom *env, Cell *args);
	Union char_lt_ci(ExAtom *env, Cell *args);
	Union char_eq_ci(ExAtom *env, Cell *args);
	Union char_ge_ci(ExAtom *env, Cell *args);
	Union char_gt_ci(ExAtom *env, Cell *args);

	Union char_downcase(ExAtom *env, Cell *args);
	Union char_upcase(ExAtom *env, Cell *args);
	Union char_lower_case_p(ExAtom *env, Cell *args);
	Union char_upper_case_p(ExAtom *env, Cell *args);
	Union char_numeric_p(ExAtom *env, Cell *args);
	Union char_whitespace_p(ExAtom *env, Cell *args);

	Union number2string(ExAtom *env, Cell *args);
	Union string_by_char(ExAtom *env, Cell *args);
	Union string_copy(ExAtom *env, Cell *args);
	Union string_append(ExAtom *env, Cell *args);
	Union string_length(ExAtom *env, Cell *args);
	Union string_ref(ExAtom *env, Cell *args);
	Union string_set(ExAtom *env, Cell *args);
	Union substring(ExAtom *env, Cell *args);
	Union string2number(ExAtom *env, Cell *args);
	Union string2symbol(ExAtom *env, Cell *args);
	Union symbol2string(ExAtom *env, Cell *args);

	Union string_ci_lt_p(ExAtom *env, Cell *args);
	Union string_ci_le_p(ExAtom *env, Cell *args);
	Union string_ci_eq_p(ExAtom *env, Cell *args);
	Union string_ci_ge_p(ExAtom *env, Cell *args);
	Union string_ci_gt_p(ExAtom *env, Cell *args);
	Union string_lt_p(ExAtom *env, Cell *args);
	Union string_le_p(ExAtom *env, Cell *args);
	Union string_eq_p(ExAtom *env, Cell *args);
	Union string_ge_p(ExAtom *env, Cell *args);
	Union string_gt_p(ExAtom *env, Cell *args);

	Union cAr(ExAtom *env, Cell *args);
	Union cDr(ExAtom *env, Cell *args);

	Union cAAr(ExAtom *env, Cell *args);
	Union cADr(ExAtom *env, Cell *args);
	Union cDAr(ExAtom *env, Cell *args);
	Union cDDr(ExAtom *env, Cell *args);

	Union cAAAr(ExAtom *env, Cell *args);
	Union cAADr(ExAtom *env, Cell *args);
	Union cADAr(ExAtom *env, Cell *args);
	Union cADDr(ExAtom *env, Cell *args);
	Union cDAAr(ExAtom *env, Cell *args);
	Union cDADr(ExAtom *env, Cell *args);
	Union cDDAr(ExAtom *env, Cell *args);
	Union cDDDr(ExAtom *env, Cell *args);

	Union cAAAAr(ExAtom *env, Cell *args);
	Union cAAADr(ExAtom *env, Cell *args);
	Union cAADAr(ExAtom *env, Cell *args);
	Union cAADDr(ExAtom *env, Cell *args);
	Union cADAAr(ExAtom *env, Cell *args);
	Union cADADr(ExAtom *env, Cell *args);
	Union cADDAr(ExAtom *env, Cell *args);
	Union cADDDr(ExAtom *env, Cell *args);
	Union cDAAAr(ExAtom *env, Cell *args);
	Union cDAADr(ExAtom *env, Cell *args);
	Union cDADAr(ExAtom *env, Cell *args);
	Union cDADDr(ExAtom *env, Cell *args);
	Union cDDAAr(ExAtom *env, Cell *args);
	Union cDDADr(ExAtom *env, Cell *args);
	Union cDDDAr(ExAtom *env, Cell *args);
	Union cDDDDr(ExAtom *env, Cell *args);

	Union do_display(ExAtom *env, Cell *args);
	Union newline(ExAtom *env, Cell *args);

	Union scheme_map(ExAtom *env, Cell *args);
	Union scheme_for_each(ExAtom *env, Cell *args);

	Union _with_open_driver(ExAtom *env, Cell *args);
	Union _driver_call(ExAtom *env, Cell *args);

	Union scheme_debug(ExAtom *env, Cell *args);
	Union scheme_gc_stat(ExAtom *env, Cell *args);
	Union scheme_require_gc(ExAtom *env, Cell *args);
} // namespace primitive
} // namespace scheme
} // namespace sign

namespace {

//------------------------------------------------------------------------

RegisterPrimitive::NameAndLambda nal[] = {
	{ "map",		primitive::scheme_map },
	{ "for-each",		primitive::scheme_for_each },

	{ "null?",		primitive::null_p },
	{ "pair?",		primitive::pair_p },
	{ "number?",		primitive::number_p },
	{ "integer?",		primitive::integer_p },
	{ "char?",		primitive::char_p },
	{ "boolean?",		primitive::boolean_p },
	{ "not",		primitive::scheme_not },
	{ "procedure?",		primitive::procedure_p },
	{ "string?",		primitive::string_p },
	{ "symbol?",		primitive::symbol_p },

	{ "eq?",		primitive::eq_p },
	{ "eqv?",		primitive::eqv_p },
	{ "equal?",		primitive::equal_p },

	{ "car",		primitive::cAr },
	{ "cdr",		primitive::cDr },

	{ "caar",		primitive::cAAr },
	{ "cadr",		primitive::cADr },
	{ "cdar",		primitive::cDAr },
	{ "cddr",		primitive::cDDr },

	{ "caaar",		primitive::cAAAr },
	{ "caadr",		primitive::cAADr },
	{ "cadar",		primitive::cADAr },
	{ "caddr",		primitive::cADDr },
	{ "cdaar",		primitive::cDAAr },
	{ "cdadr",		primitive::cDADr },
	{ "cddar",		primitive::cDDAr },
	{ "cdddr",		primitive::cDDDr },

	{ "caaaar",		primitive::cAAAAr },
	{ "caaadr",		primitive::cAAADr },
	{ "caadar",		primitive::cAADAr },
	{ "caaddr",		primitive::cAADDr },
	{ "cadaar",		primitive::cADAAr },
	{ "cadadr",		primitive::cADADr },
	{ "caddar",		primitive::cADDAr },
	{ "cadddr",		primitive::cADDDr },
	{ "cdaaar",		primitive::cDAAAr },
	{ "cdaadr",		primitive::cDAADr },
	{ "cdadar",		primitive::cDADAr },
	{ "cdaddr",		primitive::cDADDr },
	{ "cddaar",		primitive::cDDAAr },
	{ "cddadr",		primitive::cDDADr },
	{ "cdddar",		primitive::cDDDAr },
	{ "cddddr",		primitive::cDDDDr },

	{ "cons",		primitive::cons },
	{ "length",		primitive::length },
	{ "list",		primitive::list },
	{ "set-car!",		primitive::set_car },
	{ "set-cdr!",		primitive::set_cdr },
	{ "list-ref",		primitive::list_ref },
	{ "list?",		primitive::list_p },
	{ "append", 		primitive::append },
	{ "reverse", 		primitive::reverse },

	{ "+",			primitive::scheme_plus },
	{ "-",			primitive::scheme_minus },
	{ "*",			primitive::scheme_mul },
	{ "/",			primitive::scheme_div },
	{ "modulo",		primitive::scheme_modulo },
	{ "remainder",		primitive::scheme_remainder },

	{ "<",			primitive::scheme_less_than },
	{ "<=",			primitive::scheme_less_equal },
	{ "=",			primitive::scheme_num_equal },
	{ ">",			primitive::scheme_greater_than },
	{ ">=",			primitive::scheme_greater_equal },
	{ "odd?",		primitive::scheme_odd_p },
	{ "even?",		primitive::scheme_even_p },
	{ "negative?",		primitive::scheme_negative_p },
	{ "positive?",		primitive::scheme_positive_p },

	{ "memq",		primitive::memq },
	{ "memv",		primitive::memv },
	{ "member",		primitive::member },
	{ "assq",		primitive::assq },
	{ "assv",		primitive::assv },
	{ "assoc",		primitive::assoc },

	{ "zero?",		primitive::scheme_zero_p },
	{ "max",		primitive::scheme_max },
	{ "min",		primitive::scheme_min },
	{ "abs",		primitive::scheme_abs },

	{ "char->integer",	primitive::char2integer },
	{ "char->alphabetic?",	primitive::char_alphabetic_p },
	{ "char<=?",		primitive::char_le },
	{ "char<?",		primitive::char_lt },
	{ "char=?",		primitive::char_eq },
	{ "char>=?",		primitive::char_ge },
	{ "char>?",		primitive::char_gt },
	{ "char-ci<=?",		primitive::char_le_ci },
	{ "char-ci<?",		primitive::char_lt_ci },
	{ "char-ci=?",		primitive::char_eq_ci },
	{ "char-ci>=?",		primitive::char_ge_ci },
	{ "char-ci>?",		primitive::char_gt_ci },

	{ "char-downcase",	primitive::char_downcase },
	{ "char-upcase",	primitive::char_upcase },

	{ "char-lower-case?",	primitive::char_lower_case_p },
	{ "char-upper-case?",	primitive::char_upper_case_p },

	{ "char-numeric?",	primitive::char_numeric_p },
	{ "char-whitespace?",	primitive::char_whitespace_p },

	{ "display",		primitive::do_display },
	{ "newline",		primitive::newline },

	{ "number->string",	primitive::number2string },
	{ "string",		primitive::string_by_char },
	{ "string-copy",	primitive::string_copy },
	{ "string-append",	primitive::string_append },
	{ "string-length",	primitive::string_length },
	{ "string-ref",		primitive::string_ref },
	{ "string-set!",	primitive::string_set },
	{ "substring",		primitive::substring },
	{ "string->number",	primitive::string2number },
	{ "symbol->string",	primitive::symbol2string },
	{ "string->symbol",	primitive::string2symbol },

	{ "string-ci<?",	primitive::string_ci_lt_p },
	{ "string-ci<=?",	primitive::string_ci_le_p },
	{ "string-ci=?",	primitive::string_ci_eq_p },
	{ "string-ci>=?",	primitive::string_ci_ge_p },
	{ "string-ci>?",	primitive::string_ci_gt_p },

	{ "string<?",		primitive::string_lt_p },
	{ "string<=?",		primitive::string_le_p },
	{ "string=?",		primitive::string_eq_p },
	{ "string>=?",		primitive::string_ge_p },
	{ "string>?",		primitive::string_gt_p },

	{ "with-open-driver",	primitive::_with_open_driver },
	{ "driver-call",	primitive::_driver_call },

	{ "debug",		primitive::scheme_debug },
	{ "gc-stat",		primitive::scheme_gc_stat },
	{ "gc",			primitive::scheme_require_gc },

	{ "", 0 }
};

} // namespace;

namespace sign {
namespace scheme {
namespace primitive {

RegisterPrimitive base0(environment::getSystemRootEnvironment(), nal);

} // namespace primitive
} // namespace scheme
} // namespace sign

