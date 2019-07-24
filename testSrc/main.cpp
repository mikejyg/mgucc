/*
 * main.cpp
 *
 * Author: mikejyg
 */

#ifdef _WIN32
#include <mikejyg/WsaService.h>
#endif

#include <mikejyg/IntUtils.h>
#include "mikejyg/SockaddrTest.h"
#include "mikejyg/SocketTest.h"
#include "mikejyg/DatagramTest.h"
#include "mikejyg/MulticastTest.h"
#include "mikejyg/AdapterInfo.h"
#include <mikejyg/OstreamBuilder.h>

using namespace mikejyg;

int main(int argc, char **argv) {

#ifdef _WIN32
	WsaService::getInstance();
#endif

	IntUtils::test();

	SockaddrTest::test();

#ifdef _WIN32
	AdapterInfo::ipconfig();

	AdapterInfo::getAdaptersAddresses();
#endif

//	MulticastTest::getAdaptersAddresses();

	if (argc<3) {
		std::cout << "please provide arguments: multicast_group_ip interface_ip" << std::endl;
		exit(1);
	}

	// group IP, interface IP
	MulticastTest::test(argv[1], argv[2]);

	DatagramTest::test(argv[2]);

	exit(0);

	if (argc<2) {
		CoutBuilder::builder("missing argument specifying either server (-s) or client (-c).")->out();
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
