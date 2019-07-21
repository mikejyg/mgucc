/*
 * main.cpp
 *
 * Author: mikejyg
 */

#include <mikejyg/IntUtils.h>
#include "mikejyg/SockaddrTest.h"
#include "mikejyg/SocketTest.h"

using namespace mikejyg;

int main(int argc, char **argv) {
	IntUtils::test();

	SockaddrTest::test();

	if (argc<2) {
		std::cout << "missing argument specifying either server (-s) or client (-c)." << std::endl;
		exit(1);
	}

	// server arguments: -s port
	// client arguments: -c hostname port

	std::string serverOpt("-s");
	std::string clientOpt("-c");

	if ( serverOpt==argv[1] ) {
		// run server
		if (argc<3) {
			std::cout << "missing argument port to listen on." << std::endl;
			exit(1);
		}
		unsigned port = std::stoi(argv[2]);
		SocketTest::testServer(port);

	} else if ( clientOpt==argv[1] ) {
		// run client
		if (argc < 4) {
			std::cout << "missing arguments hostname port to connect to." << std::endl;
			exit(1);
		}
		// args: hostname, port
		SocketTest::testClient(argv[2], std::stoi(argv[3]));

	} else {
		std::cout << "unknown option: " << argv[1] << std::endl;
		exit(1);
	}

}
