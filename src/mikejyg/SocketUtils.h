/*
 * SocketUtils.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKETUTILS_H_
#define MIKEJYG_SOCKETUTILS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include "ErrorUtils.h"

namespace mikejyg {

/**
 * utilities for socket.
 */
class SocketUtils {
public:
	static void connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
		auto k = ::connect(sockfd, addr, addrlen);
		if (k!=0) {
			throw ErrorUtils::ErrnoException("connect() failed:");
		}
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKETUTILS_H_ */
