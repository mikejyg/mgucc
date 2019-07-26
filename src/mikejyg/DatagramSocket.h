/*
 * DatagramSocket.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_DATAGRAMSOCKET_H_
#define MIKEJYG_DATAGRAMSOCKET_H_

#include "Socket.h"
#include "DatagramPacket.h"
#include "InetSocketAddress.h"

namespace mikejyg {

/**
 * similar to Java's DatagramSocket
 */
class DatagramSocket : public Socket {
private:

public:
	virtual ~DatagramSocket() {}

	DatagramSocket(unsigned port, InetAddress const & inetAddress) {
		InetSocketAddress socketAddress(inetAddress, port);

		sockfd = SocketUtils::socket(socketAddress.getSaFamily(), SOCK_DGRAM);

		SocketUtils::bind(sockfd, socketAddress.getSockaddr(), socketAddress.getSockaddrLen());

		getsockname();
	}

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


};

} /* namespace mikejyg */

#endif /* MIKEJYG_DATAGRAMSOCKET_H_ */
