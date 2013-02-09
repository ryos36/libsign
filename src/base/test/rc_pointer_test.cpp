#include <iostream>
#include <string>
#include "sign/RCPointer.h"

using namespace sign;

class Test {
public:
	Test(int a_a, std::string a_str) : a(a_a), str(a_str) {}
	~Test() {
		a = -1;
		str = "finished";
		show_n = 0;
	}

	int a;
	std::string str;
	int show_n;

	void show(const char *c_str = 0) {
		std::cout << "------------------:" << (c_str?c_str:"") << std::endl;
		std::cout << "a:" << a << std::endl;
		std::cout << "show_n:" << show_n << std::endl;
		std::cout << "str:" << str << std::endl;
		++show_n;
	}
};

void 
test(RCPointer<Test> t = 0)
{
	if ( t ) {
		t->show("at test");
	} else {
		std::cout << "!!!!! RCPointer<Test> is null" << std::endl;
	}
}


Test *g_zangai;

RCPointer<Test>
test0()
{
	Test *t, *zangai;
	t = new Test(1, "abc");
	g_zangai = t;
	{
		Test x(1, "xyz");
		zangai = &x;
	}
	if ( 0 ) 
		zangai->show();
	RCPointer<Test> tp(t);

	test(tp);
	tp->show("at_test0");

	return tp;
}

int
main(int argc, char **argv)
{
	{
		RCPointer<Test> t;
		t = test0();
		t->show("at_main{}");
	}
	test();
	g_zangai->show("zangai");
}
