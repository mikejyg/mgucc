/*
 * FlexPtr.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_FLEXPTR_H_
#define MIKEJYG_FLEXPTR_H_

#include <memory>
#include <cassert>

namespace mikejyg {

/**
 * A flexible pointer that can either own or view a pointer.
 *
 * It combines 2 classes in one - a owning class and a view class,
 *   and allows operations to be performed on the underlying pointer, regardless.
 *
 */
template <typename T>
class FlexPtr {
protected:
	std::unique_ptr<T> uPtr;
	T * viewPtr;

public:
	/**
	 * default constructor
	 * construct an empty ptr.
	 */
	FlexPtr() : viewPtr(nullptr) {
	}

	/**
	 * construct a view pointer
	 */
	FlexPtr(const T * ptr) : viewPtr( const_cast<T*>(ptr) ) {}

	/**
	 * construct a view pointer
	 */
	FlexPtr(T * ptr) : viewPtr(ptr) {}

	/**
	 * construct a owning pointer
	 */
	FlexPtr(std::unique_ptr<T> && uPtr1) : uPtr(std::move(uPtr1)), viewPtr(uPtr.get()) {}

	/**
	 * copy constructor
	 * can only copy a view pointer.
	 */
	FlexPtr(FlexPtr const & fp2) : viewPtr(fp2.viewPtr) {
		assert(fp2.uPtr.get()==nullptr);
	}

	/**
	 * copy assignment
	 * can only copy a view pointer.
	 */
	FlexPtr & operator = (FlexPtr const & fp2) {
		assert(fp2.uPtr.get()==nullptr);

		uPtr.release();
		viewPtr = fp2.viewPtr;

		return *this;
	}

	/**
	 * move constructor
	 */
	FlexPtr(FlexPtr && fp2) : uPtr(std::move(fp2.uPtr)), viewPtr(fp2.viewPtr) {}

	/**
	 * move assignment
	 */
	FlexPtr & operator = (FlexPtr && fp2) {
		uPtr=std::move(fp2.uPtr);
		viewPtr=fp2.viewPtr;

		return *this;
	}

	/**
	 * View an external pointer.
	 * This will cause this object to release any owned pointer, if any.
	 */
	void view(T * ptr) {
		viewPtr = ptr;
		uPtr.reset();
	}

	/**
	 * take ownership of a provided pointer.
	 */
	void wrap(T * ptr) {
		uPtr.reset(ptr);
		viewPtr = ptr;
	}

	/**
	 * I'd like to use the word wrap for function name here, too, but "cannot be overloaded"
	 */
	static FlexPtr makeFlexPtrWrap(T * ptr) {
		FlexPtr fp( std::unique_ptr<T>(ptr) );
		return fp;
	}

	const T * get() const {
		return viewPtr;
	}

	T * get() {
		return viewPtr;
	}


};

}	// namespace

#endif /* MIKEJYG_FLEXPTR_H_ */
