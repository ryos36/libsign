/* Copyright (C) 2005, 2007 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_ASSERT_H__
#define __SIGN_ASSERT_H__

#if defined(_MSC_VER) || defined(__linux__) || defined(__CYGWIN__)
#define HAS_ASSERT_H
#endif

#ifdef DEBUG
	#ifdef HAS_ASSERT_H
		#include <assert.h>
		#define SIGN_ASSERT( __expr__ )	assert( __expr__ )
	#else // HAS_ASSERT_H

		#ifdef __cplusplus
		extern "C" {
		#endif
		extern int assert_on;	
		extern int assert_line;
		extern char *assert_file;

		#ifdef __GNUC__
		int assert_on __attribute__ ((weak));
		int assert_line __attribute__ ((weak));
		char *assert_file __attribute__ ((weak));
		#elif defined(__SIGN_ASSERT_VARS_DECLARE__)
		//
		// please set __SIGN_ASSERT_VARS_DECLARE__ in main.c 
		//
		int assert_on;
		int assert_line;
		char *assert_file;
		#endif

		void abort(void);

		#ifdef __cplusplus
		} // extern "C" 
		#endif

		#define SIGN_ASSERT( __expr__ )	do { \
			if ( !(__expr__) ) { \
				assert_on = 1; \
				assert_line = __LINE__; \
				assert_file = __FILE__; \
				abort(); \
			} \
		} while ( 0 );
		
	#endif // HAS_ASSERT_H

#define SIGN_COND_ASSERT( __cond__, __expr__ ) do { if ( (__cond__) ) { SIGN_ASSERT(__expr__); } } while(false)

#else // DEBUG
	#ifndef NO_ASSERT
	#define NO_ASSERT
	#endif
#endif // DEBUG

#ifdef P_DEBUG
	#define SIGN_P_ASSERT( __expr__ )	SIGN_ASSERT( __expr__ )
#else
	#define SIGN_P_ASSERT( __expr__ )
#endif

#ifdef NO_ASSERT
	#undef SIGN_ASSERT
	#define SIGN_ASSERT( __expr__ )

	#undef SIGN_COND_ASSERT
	#define SIGN_COND_ASSERT( __cond__, __expr__ )
#endif

#ifndef ASSERT
#define ASSERT( __expr__ ) SIGN_ASSERT( __expr__ )
#define P_ASSERT( __expr__ ) SIGN_P_ASSERT( __expr__ )
#endif

#endif //__SIGN_ASSERT_H__
