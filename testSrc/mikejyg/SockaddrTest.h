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

		InetSocketAddress sockaddr("localhost", 0);
		std::cout << "locahost: " << sockaddr.toString() << std::endl;

		InetSocketAddress sockaddr1("yahoo.com", 0);
		std::cout << "yahoo.com: " << sockaddr1.toString() << std::endl;

	}

};	// class

}
