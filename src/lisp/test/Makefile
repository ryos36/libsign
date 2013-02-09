#
#

TOP=../../..

include $(TOP)/mk/common.mk
CFLAGS += -std=c++0x

LOCAL_INCS=-I../include -I../src

TARGET=ExSymbolTest pair_cell_test test_gc num token_test scheme_func_test scheme_test

LIB_XCHEME=../src/libscheme.a

include $(TOP)/mk/target_all.mk

-include .deps/ExSymbolTest.d

scheme_func_test: scheme_func_test.o $(LIB_XCHEME)
	$(CXX) $(CXXFLAGS) -o scheme_func_test scheme_func_test.o $(LIB_XCHEME)

scheme_test: scheme_test.o $(LIB_XCHEME)
	$(CXX) $(CXXFLAGS) -o scheme_test scheme_test.o $(LIB_XCHEME)

test_gc: test_gc.o $(LIB_XCHEME)
	$(CXX) $(CXXFLAGS) -o test_gc test_gc.o $(LIB_XCHEME)

pair_cell_test: $(OBJECTS) pair_cell_test.o $(LIB_XCHEME)
	$(CXX) -o pair_cell_test $(OBJECTS) pair_cell_test.o $(LIB_XCHEME)

ExSymbolTest: $(OBJECTS) ExSymbolTest.o $(LIB_XCHEME)
	$(CXX) -o ExSymbolTest $(OBJECTS) ExSymbolTest.o $(LIB_XCHEME)

num: $(OBJECTS) num.o $(LIB_XCHEME)
	$(CXX) -o num $(OBJECTS) num.o $(LIB_XCHEME)

token_test: token_test.o $(LIB_XCHEME)
	$(CXX) $(CXXFLAGS) -o token_test token_test.o $(LIB_XCHEME)

etest: $(OBJECTS) etest.o $(LIB_XCHEME)
	$(CXX) -o etest $(OBJECTS) etest.o $(LIB_XCHEME)

.gitignore:Makefile
	echo $(TARGET) | xargs -n 1 > .gitignore
	echo .gdbinit >> .gitignore
