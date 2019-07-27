/*
 * Inet4Address.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INET4ADDRESS_H_
#define MIKEJYG_INET4ADDRESS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

extern "C" int InetPtonW(int Family, const char * pszAddrString, void * pAddrBuf);

#include <winsock2.h>
#include <Ws2tcpip.h>

#else
#endif

#include "InetAddress.h"
#include <cstring>
#include <stdexcept>
#include "ErrorUtils.h"
#include "AddrinfoUtils.h"
#include "SockaddrUtils.h"

namespace mikejyg {

/**
 * similar to Java's Inet4Address.
 */
class Inet4Address: public InetAddress {
public:
	virtual ~Inet4Address() {}

	Inet4Address() {}

	/**
	 * view a struct in_addr *
	 */
	Inet4Address(struct in_addr const * inAddr) : InetAddress(inAddr) {}

	/**
	 * view a struct in_addr *
	 */
	Inet4Address(struct in_addr * inAddr) : InetAddress(inAddr) {}

	/**
	 * construct from a IPv4 address string.
	 */
	Inet4Address(const char * ipv4AddressStr) {
		auto inAddrUptr = SockaddrUtils::toStructInAddr(ipv4AddressStr);
		wrap(inAddrUptr.release());
	}

	Inet4Address(std::string const & ipv4AddressStr) : Inet4Address(ipv4AddressStr.c_str()) {}

	/**
	 * get address from a hostname.
	 */
	template<typename F>
	void initFromHostname(const char * hostname, F && addrinfoSelectFunction) {
		auto * res = AddrinfoUtils::getaddrinfo(hostname, 0, AF_INET, 0, 0);
		auto * selRes = addrinfoSelectFunction(res);

		if ( selRes->ai_addr->sa_family != AF_INET )
			throw std::runtime_error("initFromHostname() addrinfo is not IPv4.");

		copy( & ((struct sockaddr_in *)selRes->ai_addr)->sin_addr );

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
		auto * newInAddr = new struct in_addr;
		memcpy(newInAddr, inAddr, sizeof(struct in_addr));
		wrap(newInAddr);
	}

	virtual std::string toString() const override {
		return SockaddrUtils::toString(get());
	}

	virtual std::unique_ptr<struct sockaddr> toStructSockaddr() const override {
		auto * sockaddrPtr = new struct sockaddr_in;
		memset( sockaddrPtr, 0, sizeof(struct sockaddr_in) );
		sockaddrPtr->sin_family=AF_INET;
		sockaddrPtr->sin_addr = *get();
		return std::unique_ptr<struct sockaddr>( (struct sockaddr*)sockaddrPtr );
	}

	virtual unsigned getStructSockaddrLen() const override {
		return sizeof(struct sockaddr_in);
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_INET4ADDRESS_H_ */
