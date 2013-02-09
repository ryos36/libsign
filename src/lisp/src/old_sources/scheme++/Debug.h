#ifndef __SIGN_SCHEME_DEBUG_H__
#define __SIGN_SCHEME_DEBUG_H__

#ifdef DEBUG
#include "sign/scheme++/Union.h"
#include <iostream>
#include <string>
void print(sign::scheme::Union a);

namespace sign {
namespace scheme {
namespace debug {

extern int flag;

} // namespace debug
} // namespace scheme
} // namespace sign

class Hello {
public:
	Hello(std::string msg = "") : msg(msg), n(0) {
		std::cerr << "Hello " << msg << ":" << n++ << std::endl;
	}

	void hi() {
		std::cerr << "hi " << msg << ":" << n++ << std::endl;
	}
	void bok() {
		std::cerr << "bok " << msg << ":" << n++ << std::endl;
	}
	void ciao() {
		std::cerr << "ciao " << msg << ":" << n++ << std::endl;
	}

	Hello & operator = (const int &v) {
		n = v;
		return *this;
	}

	Hello & operator += (const int &v) {
		n += v;
		return *this;
	}

	Hello & operator ++ () {
		++n;
		return *this;
	}

	~Hello() {
		std::cerr << "Good Bye " << msg << ":" << n++ << std::endl;
	}
private:
	const std::string msg;
	int n;
};

#define DEBUG_DO(___something_else___) do { ___something_else___ ;} while(0)
#define P_DEBUG_DO(___something_else___) 

#define DEBUG_CERR(___something_else___) do { std::cerr << ___something_else___ << std::endl; } while(0)
#define P_DEBUG_CERR(___something_else___) 

#define DEBUG_CERR_NO_ENDL(___something_else___) do { std::cerr << ___something_else___ ; } while(0)
#define P_DEBUG_CERR_NO_ENDL(___something_else___)

#else
#define DEBUG_DO(___something_else___) 
#define P_DEBUG_DO(___something_else___) 
#define DEBUG_CERR(___something_else___)
#define P_DEBUG_CERR(___something_else___) 
#define DEBUG_CERR_NO_ENDL(___something_else___)
#define P_DEBUG_CERR_NO_ENDL(___something_else___)

#endif

#endif /* __SIGN_SCHEME_DEBUG_H__ */
