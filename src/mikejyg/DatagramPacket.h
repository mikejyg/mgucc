/*
 * DatagramPacket.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_DATAGRAMPACKET_H_
#define MIKEJYG_DATAGRAMPACKET_H_

#include "SocketAddress.h"
#include <memory>
#include "FlexPtr.h"

namespace mikejyg {

/**
 * similar to Java's DatagramPacket
 */
class DatagramPacket {
protected:
	FlexPtr<char> buf;

	// buffer size
	unsigned int size;

	// data length
	unsigned int length;

	SocketAddress socketAddress;

public:

	/**
	 * Constructs a DatagramPacket for receiving packets of length length.
	 * buf is the given buffer.
	 *
	 * this is for receiving
	 */
	DatagramPacket(char * buf, int length) : buf(buf), size(length), length(length) {}

	DatagramPacket(char const * buf, int length, SocketAddress const & socketAddress)
		: buf(buf), size(length), length(length), socketAddress(socketAddress)
	{}

	const char * getBuf() const {
		return buf.get();
	}

	char * getBuf() {
		return buf.get();
	}

	unsigned int getLength() const {
		return length;
	}

	void setLength(int l) {
		length=l;
	}

	unsigned int getSize() const {
		return size;
	}

	const SocketAddress & getSocketAddress() const {
		return socketAddress;
	}

	void copySockaddr(struct sockaddr const * sockaddrPtr, int sockaddrLen) {
		socketAddress.copy(sockaddrPtr, sockaddrLen);
	}


};

} /* namespace mikejyg */

#endif /* MIKEJYG_DATAGRAMPACKET_H_ */
