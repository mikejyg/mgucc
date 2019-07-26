/*
 * StreamSocket.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef MIKEJYG_STREAMSOCKET_H_
#define MIKEJYG_STREAMSOCKET_H_

#ifdef _WIN32
#else
#include <sys/socket.h>
#endif

#include <sys/types.h>
#include "SockaddrUtils.h"
#include "SocketUtils.h"
#include "ErrorUtils.h"
#include "Socket.h"

namespace mikejyg {

/**
 * similar to Java's Socket class.
 */
class StreamSocket : public Socket {
protected:
	InetSocketAddress peerSockaddr;

public:
	virtual ~StreamSocket() {}

	StreamSocket() {}

	/**
	 * create the socket for the target host and port and connect.
	 */
	StreamSocket(InetAddress const & inAddr, unsigned port) {
		peerSockaddr.init(inAddr, port);

		// make a socket:
		sockfd = SocketUtils::socket(peerSockaddr.getSaFamily(), SOCK_STREAM);

		SocketUtils::connect(sockfd, peerSockaddr.getSockaddr(), peerSockaddr.getSockaddrLen());

		// populate socketAddress
		getsockname();
	}

	InetSocketAddress const & getPeerSocketAddress() const {
		return peerSockaddr;
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_STREAMSOCKET_H_ */
