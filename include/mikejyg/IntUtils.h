/*
 * IntUtils.h
 *
 * Author: mikejyg
 */

#ifndef MIKEJYG_INTUTILS_H_
#define MIKEJYG_INTUTILS_H_

#include <cstdlib>
#include <cassert>
#include <iostream>

namespace mikejyg {

/**
 * integer utilities.
 */
class IntUtils {
protected:

	/**
	 * scale r of [0,RAND_MAX] to [0,x)
	 * argument x must meet: x >= 1 and x <= RAND_MAX+1
	 *
	 * NOTE: When x becomes large, the results become more uneven.
	 */
	static unsigned scale(unsigned r, unsigned x) {
		unsigned d = ( (unsigned)RAND_MAX + x )  / x;
		return r / d;
	}

public:

	/**
	 * return an integer random number between 0 (inclusive) and x (exclusive).
	 * argument x must meet: x >= 1 and x <= RAND_MAX+1
	 *
	 * NOTE: When x becomes large, the results become more uneven.
	 */
	static unsigned randInt(unsigned x) {
		unsigned r = ::rand();

		return scale(r,x);
	}

	/**
	 * NOTE: call this before using any other functions in this class.
	 */
	static void sanityCheck() {

		// make sure unsigned can handle RAND_MAX + 1.
		unsigned a = RAND_MAX;
		a++;
		assert(a > (unsigned)RAND_MAX);

	}

	static void test() {
		sanityCheck();

		std::cout << "RAND_MAX: " << std::hex << RAND_MAX << std::endl;

		// test the max case of x
		unsigned x = RAND_MAX;
		x++;
		std::cout << "x=" << x << ": " << scale(RAND_MAX, x) << ", " << scale(0, x) << std::endl;

		// test min case of x
		x=1;
		std::cout << "x=" << x << ": " << scale(RAND_MAX, 1) << ", " << scale(0, x) << std::endl;

		std::cout << std::dec;

	}

};


} /* namespace mikejyg */

#endif /* MIKEJYG_INTUTILS_H_ */
