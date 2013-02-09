#ifndef __SIGN_SCHEME_ALIGN8_H__
#define __SIGN_SCHEME_ALIGN8_H__

#if defined (__GNUC__)
#define ALIGN8 __attribute__ ((aligned (8)))
#define PACKED __attribute__ ((packed))

#elif defined (_MSC_VER)

#error "sorry not implement yet"
#define PACKED
#define ALIGN8 __declspec(align(8))

#else
#error "must be 8-byte boundary"
#endif

#endif /* __SIGN_SCHEME_ALIGN8_H__ */
