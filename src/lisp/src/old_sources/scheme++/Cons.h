#ifndef __SIGN_SCHEME_CONS_H__
#define __SIGN_SCHEME_CONS_H__

#include "sign/sign_assert.h"
#include "sign/scheme++/Cell.h"
#include "sign/scheme++/Union.h"
#include <boost/iterator/iterator_facade.hpp>
#include <algorithm>
#include <boost/bind.hpp>

namespace sign {
namespace scheme {

class Cons {
public:
	template<class List, class V>
	struct iterator_gen
		: public boost::iterator_facade<iterator_gen<List, V>, V , boost::forward_traversal_tag> {
	public:
		iterator_gen(List &cons, Cell *cell = Cell::NIL) : cons(cons), cell(cell) {
		}

		V& dereference() const {
			return cell->car;
		}

		void increment() {
			SIGN_ASSERT(cell->cdr.isCell());
			cell = static_cast<Cell *>(cell->cdr);
		}

		bool equal( const iterator_gen &rhs ) const {
			SIGN_ASSERT(&cons == &rhs.cons);
			return cell == rhs.cell;
		}

		const iterator_gen<List, V> &operator =(const iterator_gen<List, V> &iter) {
			if ( &iter == this ) {
				return iter;
			}
			cell = iter.cell;
			return iter;
		}

		bool isValid() const {
			return (cell != Cell::NIL);
		}

		template<typename L2, typename V2>
		iterator_gen(const iterator_gen<L2, V2>& src )
			: cons(src.cons), cell(src.cell) {}

	public:
		//template<class, class> friend class iterator_gen;
		List &cons; // Cons!!
		Cell *cell;
	};

	typedef iterator_gen<Cons, Union> iterator;
	typedef iterator_gen<const Cons, const Union> const_iterator;
public:
	Cons(Cell *cell = 0);
	Cons(const Cons &cons);

	Cons & operator += (const Union u);
	Cons & operator += (Cell *cp);
	Cons & operator += (ExAtom *ex_atom);
	Union operator [](int n);

	void deep_copy_append(const Cell *cp);

	int length() const;
	bool isValid(int n) const;

	Cell *getTopCell();

	iterator begin();
	iterator end();
private:
	Cell *cell;
	Cell *last;

	void resetLast();
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_CONS_H__ */
