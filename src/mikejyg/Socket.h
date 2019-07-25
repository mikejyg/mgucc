/*
 * Socket.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKET_H_
#define MIKEJYG_SOCKET_H_

#include "sockfdDefs.h"
#include "SocketAddress.h"
#include <unistd.h>
#include "ErrorUtils.h"

namespace mikejyg {

/**
 * encapsulates a sockfd.
 */
class Socket {
protected:

	SockFdType sockfd;

	SocketAddress socketAddress;

public:
	Socket(SockFdType sockfd=INVALID_SOCKET) : sockfd(sockfd) {}

	void close() {
		assert(sockfd!=INVALID_SOCKET);
		::close(sockfd);
		sockfd = INVALID_SOCKET;
	}

	void setSockfd(int sockfd) {
		this->sockfd = sockfd;
	}

	int getSockfd() {
		return sockfd;
	}

	void send(const void *buf, size_t len, int flags=0) {
#ifdef _WIN32
		auto k = ::send(sockfd, (char *)buf, len, flags);
#else
		auto k = ::send(sockfd, buf, len, flags);
#endif

		if (k==-1) {
			throw ErrorUtils::ErrnoException("send() failed:");
		}
		if ( (size_t)k != len ) {
			throw std::runtime_error("send() failed: " + std::to_string(k));
		}

	}

	size_t recv(void *buf, size_t len, int flags=0) {
#ifdef _WIN32
		auto k = ::recv(sockfd, (char *)buf, len, flags);
#else
		auto k = ::recv(sockfd, buf, len, flags);
#endif
		if (k==-1) {
			throw ErrorUtils::ErrnoException("recv() failed:");
		}
		return k;

	}

	const SocketAddress & getSocketAddress() const {
		return socketAddress;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKET_H_ */
