/*
 * ErrorUtils.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_ERRORUTILS_H_
#define MIKEJYG_ERRORUTILS_H_

#include <stdexcept>

namespace mikejyg {

/**
 * various utilities for errors and exceptions.
 */
class ErrorUtils {
public:
	class ErrnoException : public std::runtime_error {
	public:
		ErrnoException(std::string const & msg) : std::runtime_error(msg + " " + strerror(errno))
		{}
	};


};

} /* namespace mikejyg */

#endif /* MIKEJYG_ERRORUTILS_H_ */
