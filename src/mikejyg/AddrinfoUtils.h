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


};

} /* namespace mikejyg */

#endif /* MIKEJYG_ADDRINFOUTILS_H_ */
