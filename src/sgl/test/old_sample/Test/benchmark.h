#ifndef _BENCH_MARK_H_
#define _BENCH_MARK_H_

#ifdef _MSC_VER

#include <iostream>
#include <windows.h>

//使い方 ファイルの先頭、もしくは関数の先頭で BENCH_INIT(); とする
//処理時間を計測したい部分の先頭で BENCH_START();
//終端で BENCH_END(func) とする


//BenchMark
#ifdef _DEBUG
	#define BENCH_INIT() double __result_time__;\
				   LARGE_INTEGER __start__, __end__, __freq__;\
	
	#define BENCH_START() ::QueryPerformanceCounter( &__start__ )

	#define BENCH_END(process_name) ::QueryPerformanceCounter( &__end__ );\
						::QueryPerformanceFrequency( &__freq__ );\
						__result_time__ = (double)(__end__.QuadPart - __start__.QuadPart) / (double)__freq__.QuadPart;\
						std::cout << process_name << " = " << __result_time__ << std::endl

#else //_DEBUG
	#define BENCH_INIT __noop
	#define BENCH_START __noop
	#define BENCH_END(func) __noop
#endif //_DEBUG

#else // _MSC_VER
	#define BENCH_INIT 
	#define BENCH_START 
	#define BENCH_END(func) ((void)0)
#endif // _MSC_VER

#endif //_BENCH_MARK_H_