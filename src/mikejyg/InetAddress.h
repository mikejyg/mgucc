/*
 * InetAddress.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_INETADDRESS_H_
#define MIKEJYG_INETADDRESS_H_

#include <netinet/in.h>
#include "FlexPtr.h"
#include <vector>
#include <string>
#include <netdb.h>

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

};

}	// namespace

#endif /* MIKEJYG_INETADDRESS_H_ */
