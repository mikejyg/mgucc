/*
 * SocketUtils.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKETUTILS_H_
#define MIKEJYG_SOCKETUTILS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include <sys/types.h>
#include <stdexcept>
#include "ErrorUtils.h"
#include "SocketAddress.h"
#include "InetSocketAddress.h"

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

	static SocketAddress getsockname(int sockfd) {
		struct sockaddr_storage address;
		socklen_t address_len = sizeof(struct sockaddr_storage);

		auto k = ::getsockname(sockfd, (struct sockaddr*)&address, &address_len);
		if (k!=0)
			throw ErrorUtils::ErrnoException("getsockname() failed:");

		SocketAddress sockaddr;
		sockaddr.copy((struct sockaddr*)&address, address_len);
		return sockaddr;

	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKETUTILS_H_ */
