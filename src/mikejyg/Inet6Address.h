/*
 * Inet6Address.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INET6ADDRESS_H_
#define MIKEJYG_INET6ADDRESS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#else
#endif

#include "InetAddress.h"
#include <cstring>

namespace mikejyg {

/**
 * Similar to Java's Inet6Address.
 */
class Inet6Address: public InetAddress {
public:
	Inet6Address() {}

	Inet6Address(struct in6_addr const * in6AddrPtr) : InetAddress( (struct in_addr *) in6AddrPtr ) {}

	Inet6Address(struct in6_addr * in6AddrPtr) : InetAddress( (struct in_addr *) in6AddrPtr ) {}

	virtual ~Inet6Address() {}

	/**
	 * get address from a hostname.
	 */
	template<typename F>
	void initFromHostname(const char * hostname, F && addrinfoSelectFunction) {
		auto * res = AddrinfoUtils::getaddrinfo(hostname, 0, AF_INET6, 0, 0);
		auto * selRes = addrinfoSelectFunction(res);

		if ( selRes->ai_addr->sa_family != AF_INET6 )
			throw std::runtime_error("initFromHostname() addrinfo is not IPv6.");

		copy( (struct in_addr const *) & ((struct sockaddr_in6 *)selRes->ai_addr)->sin6_addr );
		freeaddrinfo(res);
	}

	template<typename F>
	void initFromHostname(std::string const & hostname, F && addrinfoSelectFunction) {
		initFromHostname(hostname.c_str(), std::forward<F>(addrinfoSelectFunction));
	}

	void initFromHostname(const char * hostname) {
		initFromHostname(hostname, [](struct addrinfo const * res){ return res;});
	}

	void initFromHostname(std::string const & hostname) {
		initFromHostname(hostname.c_str());
	}

	virtual void copy(struct in_addr const * inAddr) override {
		auto * newInAddr = new struct in6_addr;
		memcpy(newInAddr, inAddr, sizeof(struct in6_addr));
		wrap( (struct in_addr *)newInAddr );
	}

	virtual std::string toString() const override {
		return SockaddrUtils::toString((struct in6_addr *)get());
	}

	virtual std::unique_ptr<struct sockaddr> toStructSockaddr() const override {
		auto * sockaddrPtr = new struct sockaddr_in6;
		memset( sockaddrPtr, 0, sizeof(struct sockaddr_in6) );
		sockaddrPtr->sin6_family=AF_INET6;
		memcpy( & sockaddrPtr->sin6_addr, get(), sizeof(struct in6_addr) );
		return std::unique_ptr<struct sockaddr>( (struct sockaddr*)sockaddrPtr );
	}

	virtual unsigned getStructSockaddrLen() const override {
		return sizeof(struct sockaddr_in6);
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_INET6ADDRESS_H_ */
