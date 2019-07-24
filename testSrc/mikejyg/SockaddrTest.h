/*
 * SockaddrTest.h
 *
 * Author: mikejyg
 */

#include <mikejyg/InetSocketAddress.h>
#include <iostream>

using namespace std;

namespace mikejyg {

/**
 * test the socket address classes
 */
class SockaddrTest {
protected:
	static void doSomething(struct sockaddr & sa) {

	}

public:

	static void test() {

		std::cout << "sizeof(struct sockaddr): " << sizeof(struct sockaddr) << std::endl;
		std::cout << "sizeof(struct sockaddr_in): " << sizeof(struct sockaddr_in) << std::endl;
		std::cout << "sizeof(struct sockaddr_in6): " << sizeof(struct sockaddr_in6) << std::endl;
		std::cout << "sizeof(struct sockaddr_storage): " << sizeof(struct sockaddr_storage) << std::endl;

		InetSocketAddress sockaddr;

		std::cout << "locahost:" << std::endl;
		sockaddr.init("localhost", 0);
		std::cout << SockaddrUtils::toString( sockaddr.getAddrinfo() );

		std::cout << "yahoo.com:" << std::endl;
		sockaddr.init("yahoo.com", 0);
		std::cout << SockaddrUtils::toString( sockaddr.getAddrinfo() );

		// test Inet4Address::toInAddr
		auto inAddr = Inet4Address::toInAddr("127.0.0.1");
		std::cout << "s_addr of 127.0.0.1: 0x" << std::hex << inAddr->s_addr << std::endl;
		assert( inAddr->s_addr == htonl(127*256*256*256+1) );

	}

};	// class

}
