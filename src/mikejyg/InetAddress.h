/*
 * InetAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INETADDRESS_H_
#define MIKEJYG_INETADDRESS_H_


#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#else
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "FlexPtr.h"
#include <vector>
#include <string>
#include <memory>

namespace mikejyg {

/**
 * similar to Java's InetAddress
 *
 * It holds a pointer to struct in_addr, or in6_addr,
 *  despite the type is in_addr.
 */
class InetAddress : public FlexPtr<in_addr> {
protected:
	InetAddress() {};

public:

	InetAddress(struct in_addr const * inAddr) : FlexPtr(inAddr) {}

	InetAddress(struct in_addr * inAddr) : FlexPtr(inAddr) {}

	virtual ~InetAddress() {}

	virtual std::string toString() const = 0;

	/**
	 * make a matching struct sockaddr.
	 */
	virtual std::unique_ptr<struct sockaddr> toStructSockaddr() const=0;

	/**
	 * length of the matching struct sockaddr
	 */
	virtual unsigned getStructSockaddrLen() const=0;

	/**
	 * copy a given inAddr - it can be in_addr, or in6_addr.
	 */
	virtual void copy(struct in_addr const * inAddr) = 0;

};

}	// namespace

#endif /* MIKEJYG_INETADDRESS_H_ */
