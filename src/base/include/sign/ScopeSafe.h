#ifndef __SIGN_SCOPE_SAFE_H__
#define __SIGN_SCOPE_SAFE_H__

namespace sign {
//------------------------------------------------------------------------
template <class T>
class ScopeSafe {
public:
	typedef void ( T::*void_func_t)();
	ScopeSafe(T *instance, void_func_t start_func, void_func_t end_func):
		instance(instance),
		start_func(start_func),
		end_func(end_func)
	{
		if ( instance ) {
			(instance->*start_func)();
		}
	}

	~ScopeSafe() {
		if ( instance ) {
			(instance->*end_func)();
		}
	}


	T *instance;
	void_func_t start_func;
	void_func_t end_func;
};

} namespace // sign

#endif /* __SIGN_SCOPE_SAFE_H__ */
