/*
 * InetSocketAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INETSOCKETADDRESS_H_
#define MIKEJYG_INETSOCKETADDRESS_H_

#include "SocketAddress.h"
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Inet4Address.h"
#include "Inet6Address.h"

namespace mikejyg {

/**
 * Similar to Java's InetSocketAddress
 *
 * It represents an internet socket address, can be either IPV4 or IPV6.
 *
 * It holds a pointer to struct sockaddr_in, or sockaddr_in6.
 *
 */
class InetSocketAddress : public SocketAddress {
protected:

	/**
	 * init with a IPV4 socket address
	 */
//	struct sockaddr_in * initStructSockaddrIn() {
//		auto * sockaddrInPtr = new struct sockaddr_in;
//		memset(sockaddrInPtr, 0, sizeof(sockaddr_in));
//
//		wrap( (struct sockaddr *) sockaddrInPtr );
//
//		setSaFamily(SocketAddress::Inet);
//
//		return sockaddrInPtr;
//	}

public:
	InetSocketAddress() {}

	virtual ~InetSocketAddress() {}

	/**
	 * Creates a socket address where the IP address is the wildcard address and the port number a specified value.
	 *
	 * This creates a IPV4 address.
	 */
	InetSocketAddress(unsigned port) {
		auto * sockaddrInPtr = new struct sockaddr_in;
		memset(sockaddrInPtr, 0, sizeof(sockaddr_in));

		wrap( (struct sockaddr *) sockaddrInPtr );

		setSaFamily(SocketAddress::Inet);
		setPort(port);

	}

	/**
	 * Creates a socket address from a hostname and a port number.
	 */
	InetSocketAddress(std::string hostname, unsigned port) {
		auto portStr = std::to_string(port);

		struct addrinfo *res;

		auto k = getaddrinfo(hostname.c_str(), portStr.c_str(),
				nullptr, & res);

		if (k!=0) {
			throw std::runtime_error("getaddrinfo() failed, return code: " + std::to_string(k));
		}

//		struct addrinfo {
//		               int              ai_flags;
//		               int              ai_family;
//		               int              ai_socktype;
//		               int              ai_protocol;
//		               socklen_t        ai_addrlen;
//		               struct sockaddr *ai_addr;
//		               char            *ai_canonname;
//		               struct addrinfo *ai_next;
//		           };

		// find the first addrinfo that is INET

		auto * addrinfoPtr = res;
		while (addrinfoPtr!=nullptr) {
			auto aiFamily = addrinfoPtr->ai_family;
			if (aiFamily==AF_INET || aiFamily==AF_INET6)
				break;

			addrinfoPtr = addrinfoPtr->ai_next;
		}

		if (addrinfoPtr==nullptr)
			throw std::runtime_error("no INET address found.");

		switch (addrinfoPtr->ai_family) {
		case AF_INET: {
			auto * sockaddrInPtr = new struct sockaddr_in;
			memcpy(sockaddrInPtr, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
			wrap( (struct sockaddr *) sockaddrInPtr );
			break;
		}

		case AF_INET6: {
			auto * sockaddrInPtr = new struct sockaddr_in6;
			memcpy(sockaddrInPtr, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
			wrap( (struct sockaddr *) sockaddrInPtr );
			break;
		}

		default: throw std::runtime_error("unknown ai_family: " + std::to_string(addrinfoPtr->ai_family));

		}

		freeaddrinfo(res);

	}

	void setPort(unsigned port) {
		((struct sockaddr_in *)get())->sin_port = htons(port);
	}

	virtual std::string toString() const override {
		return toString(get());
	}

	static std::string toString(struct sockaddr const * sockaddrPtr) {
		std::string str;

		switch (sockaddrPtr->sa_family) {
		case AF_INET: {
			auto * inet4SockaddrPtr = (struct sockaddr_in *) sockaddrPtr;
			Inet4Address inet4Address(&	inet4SockaddrPtr->sin_addr);
			str = inet4Address.toString();
			str += ":" + std::to_string( ntohs(inet4SockaddrPtr->sin_port) );
			break;
		}

		case AF_INET6: {
			auto * inet6SockaddrPtr = (struct sockaddr_in6 *) sockaddrPtr;
			Inet6Address inet6Address(&	inet6SockaddrPtr->sin6_addr);
			str = inet6Address.toString();
			str += ":" + std::to_string( ntohs(inet6SockaddrPtr->sin6_port) );
			break;
		}

		default:
			throw std::runtime_error("unknown sa_family: " + std::to_string(sockaddrPtr->sa_family));

		}

		return str;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_INETSOCKETADDRESS_H_ */
