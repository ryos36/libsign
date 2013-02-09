/*
 Copyright (c) 2001-2003, 2007 Sinby Corporation, All Rights Reserved
*/
#ifndef __SIGN_TYPES_H__
#define __SIGN_TYPES_H__

#ifndef __cplusplus
typedef int bool;
#define true (-1)
#define false (0)
#endif

#ifdef HAS_64BIT_INT
#ifdef __GNUC__
typedef unsigned long long int64;
typedef unsigned long long uint64;
#elif _MSC_VER
// Windows
typedef __int64 int64;
typedef __int64 uint64;
#else
#error "no 64 bit int declare"
#endif
#endif

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef int int32;
typedef short int16;
typedef signed char int8;

#define round4(__n__) ((((unsigned int)(__n__)) + 3) & ~3)
#define __SINBY_S_MAGIC_NO__ 20010104

#ifdef __GNUC__
	#include <sys/param.h>
#elif _MSC_VER
	#ifdef _M_IX86
	#define BIG_ENDIAN	4321
	#define LITTLE_ENDIAN	1234

	#define BYTE_ORDER	LITTLE_ENDIAN
	#endif
#elif GAIO
	// GAIO では本当はエンディアンを指定できる。
	#define BIG_ENDIAN	4321
	#define LITTLE_ENDIAN	1234

	#define BYTE_ORDER	LITTLE_ENDIAN
#endif

#ifndef BYTE_ORDER
#error "set BYTE_ORDER to LITTLE_ENDIAN or BIG_ENDIAN"
#endif

#ifdef __GNUC__
#define __GNUC_PACK_ATTR__ __attribute__ ((packed))
#define INLINE inline
#elif _MSC_VER
#define __GNUC_PACK_ATTR__ 
#define INLINE __inline
#elif GAIO
#define INLINE inline
#else
#error "I don't know your compiler"
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define BTN_TO_H_SHORT(__s__) \
	((((__s__) & 0xFF00) >> 8) | (((__s__) & 0x00FF) << 8))
#define H_TO_BTN_SHORT(__s__) \
	((((__s__) & 0xFF00) >> 8) | (((__s__) & 0x00FF) << 8))
#else
// Little Endian
#define BTN_TO_H_SHORT(__s__) (__s__)
#define H_TO_BTN_SHORT(__s__) (__s__)
#endif

#endif /* __SIGN_TYPES_H__ */
