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
#include "Socket.h"

namespace mikejyg {

/**
 * similar to Java's Socket class.
 */
class StreamSocket : public Socket {
protected:
	struct addrinfo * resSel;

public:
	StreamSocket() : resSel(nullptr) {}

	StreamSocket(StreamSocket && ss2) : Socket(std::move(ss2)), resSel(nullptr) {
	}

	StreamSocket & operator = (StreamSocket && ss2) {
		Socket::operator=( std::move(ss2) );
		return *this;
	}

	/**
	 * create the socket for the target host and port and connect, but do not connect yet.
	 */
	StreamSocket(char const * hostname, unsigned port) {
		struct addrinfo hints;

		// first, load up address structs with getaddrinfo():

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
		hints.ai_socktype = SOCK_STREAM;

		res = SockaddrUtils::getaddrinfo(hostname, port, &hints);

		// choose IPV4 if possible.

		resSel=res;
		while (resSel!=nullptr) {
			if ( resSel->ai_family == AF_INET )
				break;
			resSel=resSel->ai_next;
		}

		if (resSel==nullptr)
			resSel=res;

		// make a socket:

		sockfd = socket(resSel->ai_family, resSel->ai_socktype, resSel->ai_protocol);

		socketAddress.copy(resSel->ai_addr, resSel->ai_addrlen);

		if (sockfd==-1)
			throw ErrorUtils::ErrnoException("socket() failed:");

	}

	void connect() {
		// connect it to the address and port we passed in to getaddrinfo():

		SocketUtils::connect(sockfd, resSel->ai_addr, resSel->ai_addrlen);
	}

	virtual ~StreamSocket() {}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_STREAMSOCKET_H_ */
