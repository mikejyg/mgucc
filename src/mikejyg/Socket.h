/*
 * Socket.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKET_H_
#define MIKEJYG_SOCKET_H_

#include "SocketAddress.h"
#include <unistd.h>
#include "ErrorUtils.h"

namespace mikejyg {

/**
 * encapsulates a sockfd.
 */
class Socket {
protected:
	int sockfd;

	// working variables

	SocketAddress socketAddress;

	struct addrinfo * res;

public:
	Socket() : sockfd(-1), res(nullptr) {}

	Socket(Socket && s2) : sockfd(s2.sockfd), socketAddress(std::move(s2.socketAddress)), res(s2.res) {
		s2.sockfd=-1;
		s2.res=nullptr;
	}

	Socket & operator = (Socket && s2) {
		sockfd=s2.sockfd;
		socketAddress=std::move(s2.socketAddress);
		s2.sockfd=-1;
		return *this;
	}

	Socket(Socket const &) = delete;

	virtual ~Socket() {
		// auto close
		if (sockfd!=-1)
			close();

		if (res!=nullptr)
			freeaddrinfo(res);
	}

	void wrap(int sockfd) {
		this->sockfd = sockfd;
	}

	void close() {
		::close(sockfd);
		sockfd = 0;
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

	const SocketAddress& getSocketAddress() const {
		return socketAddress;
	}

	const struct addrinfo * getAddrinfo() const {
		return res;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKET_H_ */
