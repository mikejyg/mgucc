/*
 * MulticastSocket.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_MULTICASTSOCKET_H_
#define MIKEJYG_MULTICASTSOCKET_H_

#include "DatagramSocket.h"
#include <set>
#include <cstdint>
#include "ErrorUtils.h"

namespace mikejyg {

/**
 * Similar to Java's MulticastSocket
 * only support ipv4 for now
 */
class MulticastSocket: public DatagramSocket {
protected:
	std::set<uint32_t> joinedAddrSet;

	////////////////////////////////////////////////////////////////////

	struct ip_mreq createMcReq(uint32_t ipv4Addr) {
		struct ip_mreq mc_req;

		/* construct an IGMP join request structure */
		mc_req.imr_multiaddr.s_addr = ipv4Addr;

//		mc_req.imr_interface.s_addr = htonl(INADDR_ANY);
		mc_req.imr_interface.s_addr = ((struct sockaddr_in *)socketAddress.getSockaddr())->sin_addr.s_addr;

		return mc_req;
	}

#ifdef _WIN32
	typedef const char OptvalType;
#else
	typedef int OptvalType;
#endif

public:
	virtual ~MulticastSocket() {}

	MulticastSocket(unsigned port, InetAddress const & inetAddress) : DatagramSocket(port, inetAddress) {
		setTimeToLive(1);
	}

	MulticastSocket(int aiFamilyHint) : DatagramSocket(aiFamilyHint) {
		setTimeToLive(1);
	}

	MulticastSocket(unsigned port, int aiFamilyHint) : DatagramSocket(port, aiFamilyHint) {
		setTimeToLive(1);
	}

#ifndef _WIN32
	void setPriority(int priority) {
		if ((setsockopt(sockfd, SOL_SOCKET, SO_PRIORITY, &priority, sizeof(priority))) < 0){
			throw ErrorUtils::ErrnoException( "setsockopt() failed:" );
		}
	}
#endif

	void joinGroup(struct in_addr & inAddr) {
		if ( joinedAddrSet.find(inAddr.s_addr) != joinedAddrSet.end() ) return;

		auto mc_req = createMcReq(inAddr.s_addr);

		/* send an ADD MEMBERSHIP message via setsockopt */
		if ((setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &mc_req, sizeof(mc_req))) < 0) {
			throw ErrorUtils::ErrnoException( "setsockopt() failed:" );
		}
		joinedAddrSet.insert(inAddr.s_addr);
	}

	void leaveGroup(struct in_addr & inAddr) {
		if ( joinedAddrSet.find(inAddr.s_addr) != joinedAddrSet.end() ) return;

		auto mc_req = createMcReq(inAddr.s_addr);

		/* send a DROP MEMBERSHIP message via setsockopt */
		if ((setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*) &mc_req, sizeof(mc_req))) < 0) {
			throw ErrorUtils::ErrnoException( "setsockopt() failed:" );
		}
		joinedAddrSet.erase(inAddr.s_addr);
	}

	void setTimeToLive(unsigned char ttl) {
		/* set the TTL (time to live/hop count) for the send */
		if ((setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (const char*) &ttl, sizeof(ttl))) < 0) {
			throw ErrorUtils::ErrnoException( "setsockopt() failed:" );
		}
	}

	const std::set<uint32_t>& getJoinedAddrSet() const {
		return joinedAddrSet;
	}

	void setSocketReuseaddr() {
		OptvalType flag_on = 1;              /* socket option flag */

		/* set reuse port to on to allow multiple binds per host */
		if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag_on, sizeof(flag_on))) < 0) {
			throw ErrorUtils::ErrnoException( "setsockopt() failed:" );
		}

		setTimeToLive(1);
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_MULTICASTSOCKET_H_ */
