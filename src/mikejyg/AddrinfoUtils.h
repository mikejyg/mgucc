/*
 * AddrinfoUtils.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_ADDRINFOUTILS_H_
#define MIKEJYG_ADDRINFOUTILS_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#else
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <string>
#include <cstring>

namespace mikejyg {

class AddrinfoUtils {
public:
	static struct addrinfo * getaddrinfo (const char * hostname, unsigned port, const struct addrinfo * hint) {
		struct addrinfo *res;

		auto portStr = std::to_string(port);

		auto k = ::getaddrinfo(hostname, portStr.c_str(), hint, & res);

		if (k!=0) {
			throw std::runtime_error("getaddrinfo() failed, return code: " + std::to_string(k));
		}

		return res;
	}

	static struct addrinfo * getaddrinfo (const char * hostname, unsigned port
			, int family, int socktype, int flags) {
		struct addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = family;
		hints.ai_socktype = socktype;
		hints.ai_flags = flags;     // fill in my IP for me

		return getaddrinfo(hostname, port, & hints);
	}

	static struct addrinfo const * selectAddrinfoByFamily(struct addrinfo const * res, int af_family) {
		while (res!=nullptr) {
			if (res->ai_family==af_family)
				break;
			res=res->ai_next;
		}
		return res;
	}

	/**
	 * get addrinfo for a TCP client socket.
	 */
	static addrinfo * getStreamSocketAddrinfo(const char * hostname, unsigned port, int family) {
		return getaddrinfo(hostname, port, family, SOCK_STREAM, 0);
	}

	static std::string toString(struct addrinfo const & ai) {
		std::string str;

		auto aiFamily = ai.ai_family;

		switch (aiFamily) {
		case AF_INET:
		case AF_INET6:
			str += SockaddrUtils::toString( ai.ai_addr );
			break;

		default:
			str += "unknown ai_family: " + std::to_string(aiFamily);
		}

		switch (ai.ai_socktype) {
		case SOCK_DGRAM:
			str += ", SOCK_DGRAM"; break;

		case SOCK_STREAM:
			str += ", SOCK_STREAM"; break;

		case SOCK_RAW:
			str += ", SOCK_RAW"; break;

		default:
			str += ", unknown socktype: " + std::to_string(ai.ai_socktype); break;
		}

		switch (ai.ai_protocol) {
		case IPPROTO_TCP:
			str += ", IPPROTO_TCP"; break;
		case IPPROTO_UDP:
			str += ", IPPROTO_UDP"; break;
		case IPPROTO_IP:
			str += ", IPPROTO_IP"; break;
		default:
			str += ", unknown protocol: " + std::to_string(ai.ai_protocol); break;
		}

		if ( ai.ai_canonname != nullptr ) {
			str += ", ";
			str += ai.ai_canonname;
		}

		return str;
	}

	static std::string toString(struct addrinfo const * res) {
		std::string str;
		while (res) {
			str += toString(*res);
			str += "\n";
			res=res->ai_next;
		}
		return str;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_ADDRINFOUTILS_H_ */
