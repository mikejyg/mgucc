/*
 * DatagramSocket.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_DATAGRAMSOCKET_H_
#define MIKEJYG_DATAGRAMSOCKET_H_

#include "Socket.h"
#include "DatagramPacket.h"

namespace mikejyg {

/**
 * similar to Java's DatagramSocket
 */
class DatagramSocket : public Socket {
private:
	struct addrinfo const * resSel;

public:
	virtual ~DatagramSocket() {}

	/**
	 * Constructs a datagram socket and binds it to the specified port on the local host machine.
	 */
	DatagramSocket(unsigned port, int aiFamilyHint) {
		struct addrinfo hints;

		// first, load up address structs with getaddrinfo():

		memset(&hints, 0, sizeof hints);

		hints.ai_family = aiFamilyHint;  // use IPv4 or IPv6, whichever

		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

		res = SockaddrUtils::getaddrinfo(nullptr, port, & hints);

		// select the first result
		resSel=res;

		// make a socket:

		sockfd = socket(resSel->ai_family, resSel->ai_socktype, resSel->ai_protocol);

		// bind it to the port we passed in to getaddrinfo():

		auto k = ::bind(sockfd, resSel->ai_addr, resSel->ai_addrlen);
		if (k!=0)
			throw ErrorUtils::ErrnoException("bind() failed:");

		socketAddress.copy(resSel->ai_addr, resSel->ai_addrlen);

	}

	/**
	 * Constructs a datagram socket and binds it to any available port on the local host machine.
	 */
	DatagramSocket(int aiFamilyHint) : DatagramSocket(0, aiFamilyHint)
	{}

	/**
	 * any port on ipv4 or ipv6
	 */
	DatagramSocket() : DatagramSocket(AF_UNSPEC) {}

	/**
	 * returns the number of bytes sent.
	 */
	int send(DatagramPacket const & packet, int flags=0) {
		auto k = ::sendto(sockfd, packet.getBuf(), packet.getLength(), flags
				, packet.getSocketAddress().getSockaddr(), packet.getSocketAddress().getSockaddrLen());
		if (k==-1)
			throw ErrorUtils::ErrnoException("sendto() failed:");

		return k;
	}

	/**
	 * receive into a given packet.
	 */
	void receive(DatagramPacket & packet, int flags=0) {
		struct sockaddr_storage sockaddrStorage;
		int sockaddrLen = sizeof(sockaddrStorage);

		auto k = ::recvfrom( sockfd, packet.getBuf(), packet.getSize(), flags
				, (struct sockaddr *)&sockaddrStorage, (socklen_t*)&sockaddrLen );

		if (k==-1)
			throw ErrorUtils::ErrnoException("recvfrom() failed:");

		packet.setLength(k);
		packet.copySockaddr((struct sockaddr *)&sockaddrStorage, sockaddrLen);
	}

	struct addrinfo const & getSelectedAddrinfo() const {
		return *resSel;
	}

};

} /* namespace mikejyg */

#endif /* MIKEJYG_DATAGRAMSOCKET_H_ */
