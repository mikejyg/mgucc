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
#include <arpa/inet.h>
#endif

#include "InetAddress.h"
#include <cstring>

namespace mikejyg {

/**
 * Similar to Java's Inet6Address.
 */
class Inet6Address: public InetAddress {
public:

	Inet6Address(struct in6_addr const * in6AddrPtr) : InetAddress( (struct in_addr *) in6AddrPtr ) {}

	Inet6Address(struct in6_addr * in6AddrPtr) : InetAddress( (struct in_addr *) in6AddrPtr ) {}

	virtual ~Inet6Address() {}

	virtual std::string toString() const override {
		char buf[INET6_ADDRSTRLEN];
		memset(buf, 0xff, INET6_ADDRSTRLEN);

		inet_ntop(AF_INET6, get(), buf, INET6_ADDRSTRLEN);

		return std::string(buf);

	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_INET6ADDRESS_H_ */
