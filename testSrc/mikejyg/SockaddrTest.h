/*
 * SockaddrTest.h
 *
 * Author: mikejyg
 */

#include <mikejyg/InetSocketAddress.h>
#include <iostream>

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

		// use the selection function to print out all the addrinfos.
		auto selFunc = [](struct addrinfo const * res){
			std::cout << "available addrinfos:" << std::endl;
			std::cout << AddrinfoUtils::toString( res ) << std::endl;
			return res;
		};

		std::cout << "testing making socket addresses from a hostname..." << std::endl;

		std::cout << "locahost:" << std::endl;
		sockaddr.initFromHostname("localhost", 0, selFunc);

		std::cout << "yahoo.com:" << std::endl;
		sockaddr.initFromHostname("yahoo.com", 0, selFunc);

		std::cout << "test making inet addresses from a IP..." << std::endl;

		auto inAddr = SockaddrUtils::toStructInAddr("127.0.0.1");
		std::cout << "s_addr of 127.0.0.1: 0x" << std::hex << inAddr->s_addr << std::endl;
		assert( inAddr->s_addr == htonl(127*256*256*256+1) );

		std::string ipv6Addr("1234:5678:9abc:def0::4:0");
		auto in6Addr = SockaddrUtils::toStructIn6Addr(ipv6Addr.c_str());
		std::cout << "s_addr of " << ipv6Addr << ": 0x " << std::hex;

		for (auto i=0; i<16; i++) {
			std::cout << (int) in6Addr->s6_addr[i] << " ";
		}
		std::cout << std::endl;

		auto backIn6Str = SockaddrUtils::toString(in6Addr.get());
		std::cout << "result of inet_ntop: " << backIn6Str << std::endl;
		assert( backIn6Str == ipv6Addr );

		std::cout << "test making inet addresses from a hostname..." << std::endl;

		std::cout << "ipv4 of yahoo.com:" << std::endl;
		Inet4Address inet4Address;
		inet4Address.initFromHostname("yahoo.com", selFunc);

		std::cout << "result addrinfo: " << inet4Address.toString() << std::endl;

		std::cout << "ipv6 of yahoo.com:" << std::endl;
		Inet6Address inet6Address;
		try {
			inet6Address.initFromHostname("yahoo.com", selFunc);
			std::cout << "result addrinfo: " << inet6Address.toString() << std::endl;
		} catch (std::exception & e) {
			std::cout << "failed to get IPv6 address: " << e.what() << std::endl;
		}

	}

};	// class

}
