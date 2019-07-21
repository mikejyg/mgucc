/*
 * SockaddrUtils.h
 *
 *  Created on: Jul 20, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_SOCKADDRUTILS_H_
#define MIKEJYG_SOCKADDRUTILS_H_

#include <string>
#include "Inet4Address.h"
#include "Inet6Address.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

namespace mikejyg {

/**
 * utilities for struct sockaddr.
 */
class SockaddrUtils {
public:
	static std::string toString(struct in_addr const * inAddrPtr) {
		Inet4Address inet4Address(inAddrPtr);
		return inet4Address.toString();
	}

	static std::string toString(struct in6_addr const * inAddrPtr) {
		Inet6Address inet6Address(inAddrPtr);
		return inet6Address.toString();
	}

	static std::string toString(struct sockaddr const * sockaddrPtr) {
		std::string str;

		switch (sockaddrPtr->sa_family) {
		case AF_INET: {
			auto * inet4SockaddrPtr = (struct sockaddr_in *) sockaddrPtr;
			str = toString(& inet4SockaddrPtr->sin_addr);
			str += ":" + std::to_string( ntohs(inet4SockaddrPtr->sin_port) );
			break;
		}

		case AF_INET6: {
			auto * inet6SockaddrPtr = (struct sockaddr_in6 *) sockaddrPtr;
			str = toString(& inet6SockaddrPtr->sin6_addr);
			str += ":" + std::to_string( ntohs(inet6SockaddrPtr->sin6_port) );
			break;
		}

		default:
			throw std::runtime_error("unknown sa_family: " + std::to_string(sockaddrPtr->sa_family));

		}

		return str;
	}

	static std::string toString(struct addrinfo const * res) {
		std::string str;

		auto * addrinfoPtr = res;
		while (addrinfoPtr!=nullptr) {
			auto aiFamily = addrinfoPtr->ai_family;

			switch (aiFamily) {
			case AF_INET:
			case AF_INET6:
				str += toString( addrinfoPtr->ai_addr );
				break;

			default:
				str += "unknown ai_family: " + std::to_string(aiFamily);
			}

			switch (addrinfoPtr->ai_socktype) {
			case SOCK_DGRAM:
				str += ", SOCK_DGRAM"; break;

			case SOCK_STREAM:
				str += ", SOCK_STREAM"; break;

			case SOCK_RAW:
				str += ", SOCK_RAW"; break;

			default:
				str += ", unknown socktype: " + std::to_string(addrinfoPtr->ai_socktype); break;
			}

			switch (addrinfoPtr->ai_protocol) {
			case IPPROTO_TCP:
				str += ", IPPROTO_TCP"; break;
			case IPPROTO_UDP:
				str += ", IPPROTO_UDP"; break;
			case IPPROTO_IP:
				str += ", IPPROTO_IP"; break;
			default:
				str += ", unknown protocol: " + std::to_string(addrinfoPtr->ai_protocol); break;
			}

			if ( addrinfoPtr->ai_canonname != nullptr ) {
				str += ", ";
				str += addrinfoPtr->ai_canonname;
			}

			str += "\n";
			addrinfoPtr=addrinfoPtr->ai_next;
		}

		return str;

	}

	static struct addrinfo * getaddrinfo (const char * hostname, int port, const struct addrinfo * hint) {
		struct addrinfo *res;

		auto portStr = std::to_string(port);

		auto k = ::getaddrinfo(hostname, portStr.c_str(), hint, & res);

		if (k!=0) {
			throw std::runtime_error("getaddrinfo() failed, return code: " + std::to_string(k));
		}

		return res;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_SOCKADDRUTILS_H_ */
