/* Copyright (C) 2005, 2007 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_TRACE_H__
#define __SIGN_TRACE_H__

#ifdef P_DEBUG

	#include <iostream>
	#define SIGN_TRACE(exp) std::cout << exp << std::endl

#else

	#define SIGN_TRACE(exp)

#endif

#define TRACE SIGN_TRACE

#endif //__SIGN_TRACE_H__

