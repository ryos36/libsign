/* Copyright (C) 2008 Sinby Corporation. All Rights Reserved. */

#ifndef __SIGN_RC_POINTER_H__
#define __SIGN_RC_POINTER_H__

// RCPointer is Reference Count Pointer
namespace sign {

template< typename T >
struct DeleterBase
{
	virtual ~DeleterBase() {}
	virtual void doDelete(T *) = 0;
};


template< typename T >
class RCPointer
{
public:
	RCPointer()
	: ptr_(0)
	, deleter_(0)
	, ref_count_(0)
	{
	}

	RCPointer( T *p, DeleterBase<T> *del = 0 )
	: ptr_(p)
	, deleter_(del)
	, ref_count_(0)
	{
		if ( ptr_ ) {
			ref_count_ = new unsigned int(1);
		}
	}
	
	
	virtual ~RCPointer()
	{
		release();
	}
	
	
	RCPointer( const RCPointer<T> &other )
	: ptr_(other.ptr_)
	, deleter_(other.deleter_)
	, ref_count_(other.ref_count_)
	{
		if ( ptr_ ) {
			add();
		}
	}
	
	RCPointer<T> & operator=( const RCPointer<T> &other )
	{
		if ( (this == &other) || (ptr_ == other.ptr_) ) {
			return *this;
		}
		
		if ( ptr_ ) {
			release();
		}
		
		ptr_       = other.ptr_;
		deleter_   = other.deleter_;
		ref_count_ = other.ref_count_;
		add();
		
		return *this;
	}

	template < class U >
	RCPointer( const RCPointer<U> &other ) 
	: ptr_( other.ptr_ )
	, deleter_( other.deleter_ )
	, ref_count_( other.ref_count_ )
	{
		if (ptr_) {
			ref_count_ = other.ref_count_;
			add(); 
		}
	}

	operator T*() const
	{
		return ptr_; 
	}
	
	T * operator->() const
	{
		return ptr_;
	}
	
	bool operator==(const RCPointer<T> &other) const
	{
		return ptr_ == other.ptr_;
	}
	
	bool operator!=(const RCPointer<T>& other) const 
	{
		return ptr_ != other.ptr_;
	}

	operator bool () const
	{
		return ptr_ != 0;
	}

	T *get() const
	{
		return ptr_;
	}
protected:
	void add()
	{
		if (ref_count_) {
			++(*ref_count_);
		}
	}
	
	void release()
	{
		if (ref_count_) {
			--(*ref_count_);
			if ( 0 == (*ref_count_)) {
				if (deleter_) {
					deleter_->doDelete(ptr_);
				} else {
					delete ptr_;
				}
				ptr_ = 0;
				delete ref_count_;
			}
		}
	}

	
protected:
	T *ptr_;
	DeleterBase<T> *deleter_;
	unsigned int *ref_count_;
};


//RCpointer<void>—p‚Ìdeleter
template<typename T>
struct CastDeleter : public DeleterBase<void>
{
	virtual void doDelete(void *p)
	{
		T *t = static_cast<T*>(p);
		delete t;
	}
};

} //namespace sign
#endif //__SIGN_RC_POINTER_H__

