/*
 * Inet4Address.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INET4ADDRESS_H_
#define MIKEJYG_INET4ADDRESS_H_

#include "InetAddress.h"
#include <cstring>
#include <arpa/inet.h>

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

	virtual ~Inet4Address() {}

	virtual std::string toString() const override {
		char buf[INET_ADDRSTRLEN];
		memset(buf, 0xff, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, get(), buf, INET_ADDRSTRLEN);

		return std::string(buf);

	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_INET4ADDRESS_H_ */
