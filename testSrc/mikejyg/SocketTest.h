/*
 * SocketTest.h
 *
 *  Created on: Jul 26, 2019
 *      Author: mikejyg
 */

#ifndef TESTSRC_MIKEJYG_SOCKETTEST_H_
#define TESTSRC_MIKEJYG_SOCKETTEST_H_

#include "StreamSocketTest.h"
#include "DatagramTest.h"
#include "MulticastTest.h"

#include <mikejyg/GetoptLong.h>
#include <vector>

namespace mikejyg {

class SocketTest {
protected:
	unsigned port;
	std::string interfaceIp;
	std::string destHostname;
	std::string multicastIp;

	std::vector<std::string> tests;

	////////////////////////////////////////////

	void testUdp() {
		// group IP, interface IP
		MulticastTest::test(multicastIp, interfaceIp);

		DatagramTest::test(interfaceIp, destHostname);
	}

public:
	SocketTest() : port(0), interfaceIp("0.0.0.0"), destHostname("localhost")
		, multicastIp("239.0.0.0")
	{}

	void test(int argc, char * argv[]) {
		GetoptLong getoptLong;

		getoptLong.addOption('h', "help", [&]{
				std::cout << getoptLong.toHelpString();
		}, "print help.");

		getoptLong.addOption<GetoptLong::REQUIRED_ARGUMENT>('i', "", [&](const char * interfaceIp){
			this->interfaceIp = interfaceIp;
		}, "the IP address of the interface to bind to (default 0.0.0.0).");

		getoptLong.addOption<GetoptLong::REQUIRED_ARGUMENT>('d', "", [&](const char * destHostname){
			this->destHostname = destHostname;
		}, "the destination hostname (default localhost).");

		getoptLong.addOption<GetoptLong::REQUIRED_ARGUMENT>('p', "", [&](const char * portStr){
			port = std::stoi(portStr);
		}, "the port to use (default 0).");

		getoptLong.addOption<GetoptLong::REQUIRED_ARGUMENT>('g', "", [&](const char * multicastIp){
			this->multicastIp = multicastIp;
		}, "the multicast IP to use (default 239.0.0.0).");

		getoptLong.addOption('u', "", [&]{
			tests.push_back("udpTest");
		}, "run UDP tests.");

		getoptLong.addOption('s', "", [&]{
			tests.push_back("tcpServer");
		}, "run TCP test server.");

		getoptLong.addOption('c', "", [&]{
			tests.push_back("tcpClient");
		}, "run TCP test client.");

		try {
			getoptLong.parse(argc, argv);
		} catch (GetoptLong::UnknownOptionException & e) {
			std::cout << "UnknownOptionException: " << getoptLong.getOptopt() << std::endl;
			exit(1);
		} catch (GetoptLong::NonoptionException & e) {
			std::cout << "NonoptionException: " << argv[getoptLong.getOptind()] << std::endl;
			exit(1);
		}

		for (auto test : tests) {
			if (test=="udpTest")
				testUdp();
			else if (test=="tcpServer") {
				StreamSocketTest::testServer(port);
			} else {	// tcpClient
				StreamSocketTest::testClient(destHostname, port);
			}
		}

	}

};

}

#endif /* TESTSRC_MIKEJYG_SOCKETTEST_H_ */
