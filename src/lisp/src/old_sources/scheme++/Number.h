#ifndef __SIGN_SCHEME_NUMBER_H__
#define __SIGN_SCHEME_NUMBER_H__

#include "sign/scheme++/Union.h"

namespace sign {
namespace scheme {

class Number {
public:
	Number(Union &u);

	Number &operator = (const Number &rhs);
	Number &operator = (const int &n);
	Number &operator = (const double &n);

	Number &operator + (const Number &rhs);
	Number &operator - (const Number &rhs);
	Number &operator * (const Number &rhs);
	Number &operator / (const Number &rhs);

	bool operator < (Number &rhs) const;
	bool operator <= (Number &rhs) const;
	bool operator == (Number &rhs) const;
	bool operator >= (Number &rhs) const;
	bool operator > (Number &rhs) const;

	Number &operator += (const Number &rhs) {
		Number n(u);
		(*this) = n + rhs;
		return (*this);
	}
	Number &operator -= (const Number &rhs) {
		Number n(u);
		(*this) = n - rhs;
		return (*this);
	}
	Number &operator *= (const Number &rhs) {
		Number n(u);
		(*this) = n * rhs;
		return (*this);
	}
	Number &operator /= (const Number &rhs) {
		Number n(u);
		(*this) = n / rhs;
		return (*this);
	}

	bool eqv(const int &iv);
	bool eqv(const double &dv);

	operator Union () const;
	bool isInteger() const;

	int getInteger() const;
	double getDouble() const;

	int convertToInteger() const;
	double convertToDouble() const;
	
private:
	Union &u;

	operator int () const;
	operator double () const;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_STRING_H__ */
