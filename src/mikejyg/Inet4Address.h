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

#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include "InetAddress.h"
#include <cstring>
#include <stdexcept>
#include "ErrorUtils.h"

#ifdef _WIN32
extern "C" int InetPtonW(int Family, const char * pszAddrString, void * pAddrBuf);
#endif

namespace mikejyg {

/**
 * similar to Java's Inet4Address.
 */
class Inet4Address: public InetAddress {
public:

	/**
	 * default address: 0.0.0.0
	 */
	Inet4Address() {
		auto * inAddr = new struct in_addr;
		memset(inAddr, 0, sizeof(in_addr));

		wrap(inAddr);
	}

	Inet4Address(struct in_addr const * inAddr) : InetAddress(inAddr) {}

	Inet4Address(struct in_addr * inAddr) : InetAddress(inAddr) {}

	/**
	 * construct from a IPv4 address string.
	 */
	Inet4Address(const char * ipv4AddressStr) {
		auto inAddrUptr = toInAddr(ipv4AddressStr);
		wrap(inAddrUptr.release());
	}

	virtual ~Inet4Address() {}

	virtual std::string toString() const override {
		char buf[INET_ADDRSTRLEN];
		memset(buf, 0xff, INET_ADDRSTRLEN);

		inet_ntop(AF_INET, get(), buf, INET_ADDRSTRLEN);

		return std::string(buf);

	}

	static struct std::unique_ptr<in_addr> toInAddr(const char * cp) {
		auto inAddr = new struct in_addr;
#ifdef _WIN32
		inAddr->s_addr = inet_addr(cp);
#else
		auto k = inet_aton(cp, inAddr);
		if (k==0)
			throw std::runtime_error(std::string("invalid address: ") + cp);
#endif
		return std::unique_ptr<in_addr>(inAddr);
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
