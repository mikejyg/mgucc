/*
 * SocketAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKETADDRESS_H_
#define MIKEJYG_SOCKETADDRESS_H_

#include "FlexPtr.h"
#include <netinet/in.h>
#include <cstdint>
#include "SockaddrUtils.h"

namespace mikejyg {

/**
 * similar to Java SocketAddress
 *
 * It holds a pointer to struct sockaddr.
 *
 */
class SocketAddress : public FlexPtr<struct sockaddr> {
public:
	enum SaFamily : sa_family_t {
		Inet = AF_INET	// Internet domain sockets for use with IPv4 addresses.
		, Inet6 = AF_INET6	// Internet domain sockets for use with IPv6 addresses.
		, Unix = AF_UNIX	// UNIX domain sockets.
		, Unspecified = AF_UNSPEC
	};

protected:

public:
	SocketAddress() {}

	SocketAddress(SocketAddress && sa2) : FlexPtr(std::forward<FlexPtr>(sa2))
	{}

	SocketAddress & operator = (SocketAddress && sa2)
	{
		 wrap(sa2.uPtr.release());
		 return *this;
	}

	virtual ~SocketAddress() {}

	void copy(struct sockaddr const * sockaddr, int size) {
		auto * newPtr = malloc(size);
		memcpy(newPtr, sockaddr, size);
		wrap((struct sockaddr*)newPtr);
	}

	SaFamily getSaFamily() {
		return (SaFamily) get()->sa_family;
	}

	void setSaFamily(SaFamily saFamily) {
		get()->sa_family = saFamily;
	}

	virtual std::string toString() const {
		return SockaddrUtils::toString(get());
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKETADDRESS_H_ */
