/*
 * SocketAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKETADDRESS_H_
#define MIKEJYG_SOCKETADDRESS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

#include "FlexPtr.h"
#include <cstdint>
#include "SockaddrUtils.h"

#ifdef _WIN32
typedef u_short sa_family_t;
#endif

namespace mikejyg {

/**
 * similar to Java SocketAddress
 *
 * It holds a pointer to struct sockaddr.
 *
 */
class SocketAddress {
public:
	typedef FlexPtr<struct sockaddr> FlexPtrType;

	enum SaFamily : sa_family_t {
		Inet = AF_INET	// Internet domain sockets for use with IPv4 addresses.
		, Inet6 = AF_INET6	// Internet domain sockets for use with IPv6 addresses.
		, Unix = AF_UNIX	// UNIX domain sockets.
		, Unspecified = AF_UNSPEC
	};

private:
	FlexPtrType sockaddrFptr;
	int sockaddrLen;

public:
	virtual ~SocketAddress() {}

	SocketAddress() : sockaddrLen(0) {}

	/**
	 * move constructor
	 */
	SocketAddress(SocketAddress && sa2) 
		: sockaddrFptr(std::move(sa2.sockaddrFptr)), sockaddrLen(sa2.sockaddrLen)
	{}

	/**
	 * move assignment operator
	 */
	SocketAddress & operator = (SocketAddress && sa2)
	{
		sockaddrFptr=std::move(sa2.sockaddrFptr);
		sockaddrLen = sa2.sockaddrLen;
		return *this;
	}

	/**
	 * copy constructor
	 * creates a view.
	 */
	SocketAddress(SocketAddress const & sa2) : sockaddrFptr(sa2.sockaddrFptr), sockaddrLen(sa2.sockaddrLen) {}

	/**
	 * copy assignment operator
	 * creates a view.
	 */
	SocketAddress & operator = (SocketAddress const & sa2) {
		sockaddrFptr = sa2.sockaddrFptr;
		sockaddrLen = sa2.sockaddrLen;
		return *this;
	}

	/**
	 * create a view class, based on a given struct sockaddr.
	 */
	SocketAddress(struct sockaddr const * sockaddr, int size) : sockaddrFptr(sockaddr), sockaddrLen(size)
	{}

	/**
	 * copy the content of a struct sockaddr
	 */
	void copy(struct sockaddr const * sockaddr, int size) {
		auto * newPtr = malloc(size);
		memcpy(newPtr, sockaddr, size);
		sockaddrFptr.wrap((struct sockaddr*)newPtr);
		sockaddrLen=size;
	}

	SaFamily getSaFamily() {
		return (SaFamily) getSockaddr()->sa_family;
	}

	void setSaFamily(SaFamily saFamily) {
		sockaddrFptr.get()->sa_family = saFamily;
	}

	int getSockaddrLen() const {
		return sockaddrLen;
	}

	virtual std::string toString() const {
		return SockaddrUtils::toString(getSockaddr());
	}

	void wrap(struct sockaddr * sockaddrPtr, int sockaddrLen) {
		sockaddrFptr.wrap(sockaddrPtr);
		this->sockaddrLen = sockaddrLen;
	}

	struct sockaddr const * getSockaddr() const {
		return sockaddrFptr.get();
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKETADDRESS_H_ */
