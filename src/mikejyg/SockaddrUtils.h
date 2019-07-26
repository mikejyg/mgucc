/*
 * SockaddrUtils.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKADDRUTILS_H_
#define MIKEJYG_SOCKADDRUTILS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>

// mingw is missing this one:
extern "C" const char * inet_ntop (int af, const void *src, char *dst, socklen_t size);
extern "C" int inet_pton (int af, const char *src, void *dst);

#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <sys/types.h>
#include <stdexcept>
#include <cstring>

namespace mikejyg {

/**
 * utilities for struct sockaddr.
 */
class SockaddrUtils {
public:
	static std::string toString(struct in_addr const * inAddrPtr) {
		char buf[INET_ADDRSTRLEN];
		memset(buf, 0xff, INET_ADDRSTRLEN);

		inet_ntop(AF_INET, inAddrPtr, buf, INET_ADDRSTRLEN);

		return std::string(buf);
	}

	static std::string toString(struct in6_addr const * inAddrPtr) {
		char buf[INET6_ADDRSTRLEN];
		memset(buf, 0xff, INET6_ADDRSTRLEN);

		inet_ntop(AF_INET6, inAddrPtr, buf, INET6_ADDRSTRLEN);

		return std::string(buf);
	}

	static std::string toString(struct sockaddr const * sockaddrPtr) {
		std::string str;

		switch (sockaddrPtr->sa_family) {
		case AF_INET: {
			auto * inet4SockaddrPtr = (struct sockaddr_in *) sockaddrPtr;
			str = toString(& inet4SockaddrPtr->sin_addr);
			str += " : " + std::to_string( ntohs(inet4SockaddrPtr->sin_port) );
			break;
		}

		case AF_INET6: {
			auto * inet6SockaddrPtr = (struct sockaddr_in6 *) sockaddrPtr;
			str = toString(& inet6SockaddrPtr->sin6_addr);
			str += " : " + std::to_string( ntohs(inet6SockaddrPtr->sin6_port) );
			break;
		}

		default:
			throw std::runtime_error("unknown sa_family: " + std::to_string(sockaddrPtr->sa_family));

		}

		return str;
	}

	static struct std::unique_ptr<struct in_addr> toStructInAddr(const char * ipAddressStr) {
		auto * inAddr = new struct in_addr;

		auto k = inet_pton(AF_INET, ipAddressStr, inAddr);

		if (k!=1)
			throw std::runtime_error(std::string("inet_pton failed: ") + ipAddressStr);

		return std::unique_ptr<struct in_addr>(inAddr);
	}

	static struct std::unique_ptr<struct in6_addr> toStructIn6Addr(const char * ip6AddressStr) {
		auto * in6Addr = new struct in6_addr;

		auto k = inet_pton(AF_INET6, ip6AddressStr, in6Addr);

		if (k!=1)
			throw std::runtime_error(std::string("inet_pton failed: ") + ip6AddressStr);

		return std::unique_ptr<struct in6_addr>(in6Addr);
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKADDRUTILS_H_ */
