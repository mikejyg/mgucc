/*
 * SelectUtils.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SELECTUTILS_H_
#define MIKEJYG_SELECTUTILS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#else
#include <sys/select.h>
#endif

#include "ErrorUtils.h"

namespace mikejyg {

/**
 * utilities for using select().
 */
class SelectUtils {
public:

	static void waitForRead(int sockfd) {
		fd_set rfds;

		FD_ZERO(&rfds);
		FD_SET(sockfd, &rfds);

		auto k = select(sockfd+1, &rfds, nullptr, nullptr, nullptr);

		if (k == -1)
			throw ErrorUtils::ErrnoException("select() failed:");
		else if (k==0)
			throw std::runtime_error("select() returns 0:");
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SELECTUTILS_H_ */
