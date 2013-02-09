#include <cmath>
#include <stdexcept>
#include "sign/sign_assert.h"
#include "sign/scheme++/Union.h"
#include "sign/scheme++/ExAtom.h"
#include "sign/scheme++/Number.h"
#include "sign/scheme++/Debug.h"

using namespace sign::scheme;

Number::Number(Union &u) : u(u)
{
	SIGN_ASSERT(u.isNumber() || u.isNull());
}

//------------------------------------------------------------------------
Number::operator Union () const
{
	return u;
}

//------------------------------------------------------------------------
Number &
Number::operator = (const Number &rhs)
{
	if ( rhs.u.isInteger() ) {
		int v;
		v = static_cast<int>(rhs.u);
		u = v;
		return (*this);
	}

	double d;
	d = rhs.u.getDouble();
	if ( u.isInteger()) {
		u = ExAtom::alloc_double(d);
	} else {
		u.setDouble(d);
	}
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator = (const int &iv)
{
	u = iv;
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator = (const double &dv)
{
	if ( u.isInteger()) {
		u = ExAtom::alloc_double(dv);
	} else {
		u.setDouble(dv);
	}
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator + (const Number &rhs)
{
	if ( rhs.u.isInteger() ) {
		int v;
		v = static_cast<int>(rhs.u);
		if (u.isInteger()) {
			int v0;
			v0 = static_cast<int>(u);
			u = v0 + v;
			return (*this);
		}
	}
	double d0;
	if ( u.isInteger()) {
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}

	double drv;
	drv = d0 + rhs.u.getDouble();

	if ( u.isInteger()) {
		u = ExAtom::alloc_double(drv);
	} else {
		u.setDouble(drv);
	}
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator - (const Number &rhs)
{
	if ( rhs.u.isInteger() ) {
		int v;
		v = static_cast<int>(rhs.u);
		if (u.isInteger()) {
			int v0;
			v0 = static_cast<int>(u);
			u = v0 - v;
			return (*this);
		}
	}
	double d0;
	if ( u.isInteger()) {
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}

	double drv;
	drv = d0 - rhs.u.getDouble();

	if ( u.isInteger()) {
		u = ExAtom::alloc_double(drv);
	} else {
		u.setDouble(drv);
	}
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator * (const Number &rhs)
{
	double d1;
	if ( rhs.u.isInteger() ) {
		int v;
		v = static_cast<int>(rhs.u);
		if (u.isInteger()) {
			int v0;
			v0 = static_cast<int>(u);
			u = v0 * v;
			return (*this);
		}
		d1 = v;
	} else {
		d1 = rhs.u.getDouble();
	}
	double d0;
	if ( u.isInteger()) {
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}

	double drv;
	drv = d0 * d1;

	if ( u.isInteger()) {
		u = ExAtom::alloc_double(drv);
	} else {
		u.setDouble(drv);
	}
	return (*this);
}

//------------------------------------------------------------------------
Number &
Number::operator / (const Number &rhs)
{
	if ( rhs.u.isInteger() ) {
		int v;
		v = static_cast<int>(rhs.u);
		if (u.isInteger()) {
			int v0;
			v0 = static_cast<int>(u);
			if (( v0 % v ) == 0 ) {
				u = v0 / v;
				return (*this);
			}
		}
	}
	double d0;
	if ( u.isInteger()) {
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}

	double drv;
	if ( rhs.u.isInteger() ) {
		drv = d0 / static_cast<int>(rhs.u);
	} else {
		drv = d0 / rhs.u.getDouble();
	}

	if ( u.isInteger()) {
		u = ExAtom::alloc_double(drv);
	} else {
		u.setDouble(drv);
	}
	return (*this);
}

//------------------------------------------------------------------------
bool 
Number::operator < (Number &rhs) const
{
	double d0, d1;
	if ( u.isInteger()) {
		if ( rhs.isInteger()) {
			return static_cast<int>(u) < static_cast<int>(rhs.u);
		}
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}
	if ( rhs.isInteger()) {
		d1 = static_cast<int>(rhs.u);
	} else {
		d1 = rhs.u.getDouble();
	}
	return d0 < d1;
}

//------------------------------------------------------------------------
bool
Number::operator <= (Number &rhs) const
{
	double d0, d1;
	if ( u.isInteger()) {
		if ( rhs.isInteger()) {
			return static_cast<int>(u) <= static_cast<int>(rhs.u);
		}
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}
	if ( rhs.isInteger()) {
		d1 = static_cast<int>(rhs.u);
	} else {
		d1 = rhs.u.getDouble();
	}
	return d0 <= d1;
}

//------------------------------------------------------------------------
bool
Number::operator == (Number &rhs) const
{
	double d0, d1;
	if ( u.isInteger()) {
		if ( rhs.isInteger()) {
			return static_cast<int>(u) == static_cast<int>(rhs.u);
		}
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}
	if ( rhs.isInteger()) {
		d1 = static_cast<int>(rhs.u);
	} else {
		d1 = rhs.u.getDouble();
	}
	return d0 == d1;
}

//------------------------------------------------------------------------
bool
Number::operator >= (Number &rhs) const
{
	double d0, d1;
	if ( u.isInteger()) {
		if ( rhs.isInteger()) {
			return static_cast<int>(u) >= static_cast<int>(rhs.u);
		}
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}
	if ( rhs.isInteger()) {
		d1 = static_cast<int>(rhs.u);
	} else {
		d1 = rhs.u.getDouble();
	}
	return d0 >= d1;
}

//------------------------------------------------------------------------
bool
Number::operator > (Number &rhs) const
{
	double d0, d1;
	if ( u.isInteger()) {
		if ( rhs.isInteger()) {
			return static_cast<int>(u) > static_cast<int>(rhs.u);
		}
		d0 = static_cast<int>(u);
	} else {
		d0 = u.getDouble();
	}
	if ( rhs.isInteger()) {
		d1 = static_cast<int>(rhs.u);
	} else {
		d1 = rhs.u.getDouble();
	}
	return d0 > d1;
}


//------------------------------------------------------------------------
bool
Number::eqv(const int &iv)
{
	if ( u.isInteger()) {
		return (static_cast<int>(u) == iv);
	}
	double dv0, dv1;
	dv0 = u.getDouble();
	dv1 = static_cast<double>(iv);
	return (dv0 == dv1);
}

//------------------------------------------------------------------------
bool
Number::eqv(const double &dv)
{
	double dv0;
	if ( u.isInteger()) {
		dv0 = static_cast<double>(static_cast<int>(u));
	} else {
		dv0 = u.getDouble();
	}
	return (dv0 == dv);
}

//------------------------------------------------------------------------
int
Number::getInteger() const
{
	if ( u.isInteger()) {
		return static_cast<int>(u);
	}

	double d;
	d = u.getDouble();
	if ((d - floor(d)) != 0) {
		throw std::runtime_error("invalid arg");
	}
	return static_cast<int>(d);
}

//------------------------------------------------------------------------
double
Number::getDouble() const
{
	return u.getDouble();
}

//------------------------------------------------------------------------
int
Number::convertToInteger() const
{
	if ( u.isInteger()) {
		return static_cast<int>(u);
	}

	double d;
	d = u.getDouble();
	return static_cast<int>(d);
}

//------------------------------------------------------------------------
double
Number::convertToDouble() const
{
	if ( u.isInteger()) {
		return static_cast<int>(u);
	}

	double d;
	d = u.getDouble();
	return d;
}

//------------------------------------------------------------------------
bool
Number::isInteger() const
{
	return u.isInteger();
}

//------------------------------------------------------------------------
Number::operator int () const
{
	SIGN_ASSERT(u.isInteger());
	return static_cast<int>(u);
}

//------------------------------------------------------------------------
Number::operator double () const
{
	SIGN_ASSERT(!u.isInteger());
	return u.getDouble();
}
