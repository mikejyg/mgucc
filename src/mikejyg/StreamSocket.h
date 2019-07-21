/*
 * StreamSocket.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_STREAMSOCKET_H_
#define MIKEJYG_STREAMSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "SockaddrUtils.h"
#include "SocketUtils.h"
#include "ErrorUtils.h"
#include <unistd.h>
//#include <cunistd>

namespace mikejyg {

/**
 * similar to Java's Socket class.
 */
class StreamSocket {
protected:
	int sockfd;

public:
	StreamSocket(char const * hostname, unsigned port) {
		struct addrinfo hints;

		// first, load up address structs with getaddrinfo():

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;

		auto * res = SockaddrUtils::getaddrinfo(hostname, port, &hints);

		// make a socket:

		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (sockfd==-1)
			throw ErrorUtils::ErrnoException("socket() failed:");

		freeaddrinfo(res);

		// connect it to the address and port we passed in to getaddrinfo():

		SocketUtils::connect(sockfd, res->ai_addr, res->ai_addrlen);

	}

	virtual ~StreamSocket() {
		// auto close
		if (sockfd)
			close();
	}

	int getSockfd() {
		return sockfd;
	}

	void close() {
		::close(sockfd);
		sockfd = 0;
	}

	void send(const void *buf, size_t len, int flags=0) {
		auto k = ::send(sockfd, buf, len, flags);
		if (k==-1) {
			throw ErrorUtils::ErrnoException("send() failed:");
		}
		if ( (size_t)k != len ) {
			throw std::runtime_error("send() failed: " + std::to_string(k));
		}

	}

	size_t recv(void *buf, size_t len, int flags=0) {
		auto k = ::recv(sockfd, buf, len, flags);
		if (k==-1) {
			throw ErrorUtils::ErrnoException("recv() failed:");
		}
		return k;

	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_STREAMSOCKET_H_ */
