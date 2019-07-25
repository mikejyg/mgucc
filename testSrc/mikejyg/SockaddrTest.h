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

public:

	static void test() {

		std::cout << "sizeof(struct sockaddr): " << sizeof(struct sockaddr) << std::endl;
		std::cout << "sizeof(struct sockaddr_in): " << sizeof(struct sockaddr_in) << std::endl;
		std::cout << "sizeof(struct sockaddr_in6): " << sizeof(struct sockaddr_in6) << std::endl;
		std::cout << "sizeof(struct sockaddr_storage): " << sizeof(struct sockaddr_storage) << std::endl;

		InetSocketAddress sockaddr;

		auto selFunc = [](struct addrinfo const * res){
			std::cout << SockaddrUtils::toString( res );
			return res;
		};

		std::cout << "locahost:" << std::endl;
		sockaddr.init("localhost", 0, selFunc);

		std::cout << "yahoo.com:" << std::endl;
		sockaddr.init("yahoo.com", 0, selFunc);

		// test Inet4Address::toInAddr
		auto inAddr = Inet4Address::toInAddr("127.0.0.1");
		std::cout << "s_addr of 127.0.0.1: 0x" << std::hex << inAddr->s_addr << std::endl;
		assert( inAddr->s_addr == htonl(127*256*256*256+1) );

	}

};	// class

}
