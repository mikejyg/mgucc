/*
 * ServerSocket.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SERVERSOCKET_H_
#define MIKEJYG_SERVERSOCKET_H_

#include "SockaddrUtils.h"
#include "Socket.h"
#include "ErrorUtils.h"
#include <tuple>
#include "SocketAddress.h"

namespace mikejyg {

/**
 * similar to Java's ServerSocket
 */
class ServerSocket : public Socket {
public:
	static const int DEFAULT_BACKLOG=10;
protected:
	int backlog;

public:
	ServerSocket() : backlog(DEFAULT_BACKLOG) {}

	ServerSocket(unsigned port) : ServerSocket() {

		struct addrinfo hints;

		// first, load up address structs with getaddrinfo():

		memset(&hints, 0, sizeof hints);

		hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever

		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

		res = SockaddrUtils::getaddrinfo(nullptr, port, & hints);

		// server on IPV6 will also work for IPV4, but not vice versa.
		// choose IPV6 if possible.

		auto resSel=res;
		while (resSel!=nullptr) {
			if ( resSel->ai_family == AF_INET6 )
				break;
			resSel=resSel->ai_next;
		}

		if (resSel==nullptr)
			resSel=res;

		// make a socket:

		sockfd = socket(resSel->ai_family, resSel->ai_socktype, resSel->ai_protocol);

		// bind it to the port we passed in to getaddrinfo():

		bind(sockfd, resSel->ai_addr, resSel->ai_addrlen);

		socketAddress.copy(resSel->ai_addr, resSel->ai_addrlen);

	}

	virtual ~ServerSocket() {
	}

	void listen() {
		auto k = ::listen(sockfd, backlog);
		if (k!=0)
			throw ErrorUtils::ErrnoException("listen() failed:");
	}

	void setBacklog(int backlog) {
		this->backlog = backlog;
	}

	std::tuple<StreamSocket, SocketAddress> accept() {
		std::unique_ptr<struct sockaddr_storage> sockAddrUptr(new struct sockaddr_storage);

		socklen_t addr_size = sizeof(struct sockaddr_storage);

		auto new_fd = ::accept(sockfd, (struct sockaddr *)sockAddrUptr.get(), &addr_size);

		StreamSocket streamSocket;
		streamSocket.wrap(new_fd);

		SocketAddress socketAddress;

		switch (sockAddrUptr->ss_family) {
		case AF_INET: {
			auto * sockAddr = new struct sockaddr_in;
			memcpy(sockAddr, sockAddrUptr.get(), sizeof(struct sockaddr_in));
			socketAddress.wrap( (struct sockaddr*)sockAddr );
			break;
		}
		case AF_INET6: {
			auto * sockAddr6 = new struct sockaddr_in6;
			memcpy(sockAddr6, sockAddrUptr.get(), sizeof(struct sockaddr_in6));
			socketAddress.wrap( (struct sockaddr*)sockAddr6 );
			break;
		}
		default:
			socketAddress.wrap( (struct sockaddr*)sockAddrUptr.release() );
		}

		return std::make_tuple(std::move(streamSocket), std::move(socketAddress));

	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SERVERSOCKET_H_ */
