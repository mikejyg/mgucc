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
	virtual ~ServerSocket() {
	}

	ServerSocket() : backlog(DEFAULT_BACKLOG) {}

	ServerSocket(unsigned port, InetAddress const & bindInAddr) : ServerSocket() {
		InetSocketAddress sockAddr(bindInAddr, port);

		// make a socket:
		sockfd = SocketUtils::socket(sockAddr.getSaFamily(), SOCK_STREAM);

		// bind
		SocketUtils::bind(sockfd, sockAddr.getSockaddr(), sockAddr.getSockaddrLen());

		// populate socketAddress
		socketAddress = SocketUtils::getsockname(sockfd);
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
		streamSocket.setSockfd(new_fd);

		SocketAddress socketAddress;

		switch (sockAddrUptr->ss_family) {
		case AF_INET: {
			auto * sockAddr = new struct sockaddr_in;
			memcpy(sockAddr, sockAddrUptr.get(), sizeof(struct sockaddr_in));
			socketAddress.wrap( (struct sockaddr*)sockAddr, sizeof(struct sockaddr_in) );
			break;
		}
		case AF_INET6: {
			auto * sockAddr6 = new struct sockaddr_in6;
			memcpy(sockAddr6, sockAddrUptr.get(), sizeof(struct sockaddr_in6));
			socketAddress.wrap( (struct sockaddr*)sockAddr6, sizeof(struct sockaddr_in6) );
			break;
		}
		default:
			socketAddress.wrap( (struct sockaddr*)sockAddrUptr.release(), addr_size );
		}

		return std::make_tuple(std::move(streamSocket), std::move(socketAddress));

	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SERVERSOCKET_H_ */
