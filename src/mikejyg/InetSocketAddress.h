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
	/////////////////////////////////////////////////////

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
			wrap( (struct sockaddr *) sockaddrInPtr, sizeof(struct sockaddr_in) );
			break;
		}

		case AF_INET6: {
			auto * sockaddrInPtr = new struct sockaddr_in6;
			memcpy(sockaddrInPtr, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
			wrap( (struct sockaddr *) sockaddrInPtr, sizeof(struct sockaddr_in6) );
			break;
		}

		default: throw std::runtime_error("unknown ai_family: " + std::to_string(addrinfoPtr->ai_family));

		}

	}

public:
	InetSocketAddress() {}

	virtual ~InetSocketAddress()
	{}

	/**
	 * Creates a socket address where the IP address is the wildcard address and the port number a specified value.
	 *
	 * This creates a IPV4 address.
	 */
	InetSocketAddress(unsigned port) {
		auto * sockaddrInPtr = new struct sockaddr_in;
		memset(sockaddrInPtr, 0, sizeof(sockaddr_in));

		wrap( (struct sockaddr *) sockaddrInPtr, sizeof(struct sockaddr_in) );

		setSaFamily(SocketAddress::INET);
		setPort(port);
	}

	/**
	 * construct a view object.
	 * This is for casting SocketAddress to InetSocketAddress.
	 */
	InetSocketAddress(SocketAddress const & socketAddress) : SocketAddress(socketAddress) {
		// verify
		if (getSaFamily()!=SocketAddress::INET && getSaFamily()!=SocketAddress::INET6)
			throw std::runtime_error("InetSocketAddress() socketAddress is not inet.");
	}

	/**
	 * construct from a given InetAddress and a port.
	 */
	InetSocketAddress(const InetAddress & inetAddr, unsigned port)
	{
		init(inetAddr, port);
	}

	/**
	 * init from a InetAddress and a port.
	 */
	void init(const InetAddress & inetAddr, unsigned port)
	{
		wrap( inetAddr.toStructSockaddr().release(), inetAddr.getStructSockaddrLen() );
		setPort(port);
	}

	/**
	 * initialize from a hostname and a port number.
	 *
	 * no hint is used.
	 *
	 * addrinfoSelectFunction selects a addrinfo * from a linked list of addrinfo *.
	 * 	 the type should be std::function< struct addrinfo const * (struct addrinfo const *) >
	 *
	 */
	template<typename F>
	void initFromHostname(const char * hostname, unsigned port
			, F && addrinfoSelectFunction = [](struct addrinfo const * res){ return res;} ) {
		auto * res = AddrinfoUtils::getaddrinfo(hostname, port, nullptr);

		auto * selRes = addrinfoSelectFunction(res);

		init(selRes);

		freeaddrinfo(res);
	}

	template<typename F>
	void initFromHostname(std::string const & hostname, unsigned port
			, F && addrinfoSelectFunction = [](struct addrinfo const * res){ return res;} ) {
		initFromHostname(hostname.c_str(), port, std::forward<F>(addrinfoSelectFunction));
	}

	unsigned getPort() const {
		return ntohs( ((struct sockaddr_in *)getSockaddr())->sin_port );
	}

	void setPort(unsigned port) {
		((struct sockaddr_in *)getSockaddr())->sin_port = htons(port);
	}

	std::string getInAddrStr() const {
		switch (getSaFamily()) {
		case SocketAddress::INET:
			return SockaddrUtils::toString( & ((struct sockaddr_in const *)getSockaddr())->sin_addr );

		case SocketAddress::INET6:
			return SockaddrUtils::toString( & ((struct sockaddr_in6 const *)getSockaddr())->sin6_addr );

		default:
			throw std::runtime_error( "getInAddrStr() unknown family: " + std::to_string(getSaFamily()) );
		}
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_INETSOCKETADDRESS_H_ */
