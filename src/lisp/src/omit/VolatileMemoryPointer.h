/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_VOLATILE_MEMORY_POINTER_H__
#define __SIGN_VOLATILE_MEMORY_POINTER_H__

namespace sign {

template<class T>
class VolatileMemoryPointer {
public:
	VolatileMemoryPointer(T *a_ptr) : ptr(a_ptr) {}

	explicit operator T*() {
		return ptr;
	}

	T *operator->() {
		return ptr;
	}

	void replace(T *new_ptr) {
		ptr = new_ptr;
	}
protected:
	T *ptr;
};

} // namespace sign

#endif /* __SIGN_VOLATILE_MEMORY_POINTER_H__ */
