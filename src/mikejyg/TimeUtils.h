/*
 * TimeUtils.h
 *
 *  Created on: Feb 4, 2018
 *      Author: mikejyg
 */

#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

#include <sys/time.h>
#include <functional>

namespace mikejyg {

class TimeUtils {
public:

	/**
	 * get current time in double
	 */
	static inline double getTime() {
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		double t = tv.tv_usec;
		t/=1e6;
		t+=tv.tv_sec;
		return t;
	}

	/**
	 * measure the execution time of a function
	 */
	template<typename F>
	static inline double execTime( F && f) {
		auto st = getTime();
		f();
		return getTime() - st;
	}

};

}

#endif /* TIMEUTILS_H_ */
