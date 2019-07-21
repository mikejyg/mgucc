/*
 * InetSocketAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INETSOCKETADDRESS_H_
#define MIKEJYG_INETSOCKETADDRESS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "SocketAddress.h"
#include <cstring>
#include <string>
#include <sys/types.h>
#include "Inet4Address.h"
#include "Inet6Address.h"
#include "SockaddrUtils.h"
#include <functional>

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

	std::function< struct addrinfo const * (struct addrinfo const *) > addrinfoSelectFunction;

	/////////////////////////////////////////////////////

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

	/**
	 * initialize from a struct addrinfo
	 */
	void init(struct addrinfo const * addrinfoPtr) {
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

	}

public:
	InetSocketAddress() : addrinfoSelectFunction([](struct addrinfo const * res){
		return selectAddrinfo(res); })
	{}

	virtual ~InetSocketAddress() {}

	/**
	 * Creates a socket address where the IP address is the wildcard address and the port number a specified value.
	 *
	 * This creates a IPV4 address.
	 */
	InetSocketAddress(unsigned port) : InetSocketAddress() {
		auto * sockaddrInPtr = new struct sockaddr_in;
		memset(sockaddrInPtr, 0, sizeof(sockaddr_in));

		wrap( (struct sockaddr *) sockaddrInPtr );

		setSaFamily(SocketAddress::Inet);
		setPort(port);

	}

	/**
	 * Creates a socket address from a hostname and a port number.
	 */
	InetSocketAddress(std::string hostname, unsigned port) : InetSocketAddress() {
		init(hostname, port);
	}

	/**
	 * initialize from a hostname and a port number.
	 */
	void init(std::string hostname, unsigned port) {
		auto * res = SockaddrUtils::getaddrinfo(hostname.c_str(), port, nullptr);

		auto * addrinfoPtr = addrinfoSelectFunction(res);

		init(addrinfoPtr);

		freeaddrinfo(res);
	}

	void setPort(unsigned port) {
		((struct sockaddr_in *)get())->sin_port = htons(port);
	}

	/**
	 * set the selection function
	 */
	void setAddrinfoSelectFunction(
			const std::function<const struct addrinfo* (const struct addrinfo*)> &addrinfoSelectFunction) {
		this->addrinfoSelectFunction = addrinfoSelectFunction;
	}

	/**
	 * select the first addrinfo that is INET
	 */
	static struct addrinfo const * selectAddrinfo(struct addrinfo const * res) {
		auto * addrinfoPtr = res;
		while (addrinfoPtr!=nullptr) {
			auto aiFamily = addrinfoPtr->ai_family;
			if (aiFamily==AF_INET || aiFamily==AF_INET6)
				break;

			addrinfoPtr = addrinfoPtr->ai_next;
		}

		if (addrinfoPtr==nullptr)
			throw std::runtime_error("no INET address found.");

		return addrinfoPtr;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_INETSOCKETADDRESS_H_ */
