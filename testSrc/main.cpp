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

void testUdp(int argc, char **argv, int & argIdx) {
	// parameters: -u group_IP bind_interface_IP destination_IP(unicast)

	if (argc-argIdx<3) {
		std::cout << "please provide arguments: multicast_group_ip interface_ip dest_ip" << std::endl;
		exit(1);
	}

	// group IP, interface IP
	MulticastTest::test(argv[argIdx], argv[argIdx+1]);

	DatagramTest::test(argv[argIdx+1], argv[argIdx+2]);

	argIdx+=3;
}

void testTcp(int argc, char **argv, int & argIdx) {
	// parameters: -s binding_port
	// parameters: -c destination_hostname destination_port

	if ( argc - argIdx < 2 ) {
		CoutBuilder::builder("missing argument specifying either server (-s) or client (-c).")->out();
		exit(1);
	}

	// server arguments: -s port
	// client arguments: -c hostname port

	std::string serverOpt("-s");
	std::string clientOpt("-c");

	if ( serverOpt==argv[argIdx] ) {
		argIdx++;

		// run server
		if ( argc-argIdx < 1 ) {
			std::cout << "missing argument port to listen on." << std::endl;
			exit(1);
		}
		unsigned port = std::stoi(argv[argIdx++]);
		SocketTest::testServer(port);

	} else if ( clientOpt==argv[argIdx] ) {
		argIdx++;

		// run client
		if ( argc-argIdx < 2 ) {
			std::cout << "missing arguments hostname port to connect to." << std::endl;
			exit(1);
		}
		// args: hostname, port
		SocketTest::testClient(argv[argIdx], std::stoi(argv[argIdx+1]));
		argIdx += 2;

	} else {
		std::cout << "unknown option: " << argv[argIdx] << std::endl;
		exit(1);
	}
}

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

	int argIdx=1;

	if (argc-argIdx<1) {
		std::cout << "for further testing, please provide arguments for test type: -u for udp tests, -s for tcp server, -c for tcp client" << std::endl;
		exit(0);
	}

	// first check for UDP test
	if ( strcmp("-u", argv[argIdx])==0 ) {
		argIdx++;

		testUdp(argc, argv, argIdx);

		if (argIdx>=argc)	// no more args
			exit(0);
	}

	// next check for TCP test
	testTcp(argc, argv, argIdx);

}

