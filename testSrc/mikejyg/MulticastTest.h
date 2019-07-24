/*
 * MulticastTest.h
 *
 *      Author: jgu
 */

#ifndef TESTSRC_MIKEJYG_MULTICASTTEST_H_
#define TESTSRC_MIKEJYG_MULTICASTTEST_H_

#include <mikejyg/MulticastSocket.h>
#include <iostream>
#include <mikejyg/SocketUtils.h>
#include "DatagramTest.h"

namespace mikejyg {

class MulticastTest : public DatagramTest {
public:
	MulticastTest() {}
	virtual ~MulticastTest() {}

	static void test(std::string const & groupIpStr, std::string const & interfaceIpStr) {
		std::cout << "MulticastTest::test()..." << std::endl;

		// wildcard address
//		MulticastSocket ms(49150, AF_INET);

		Inet4Address ifAddr(interfaceIpStr.c_str());
		MulticastSocket ms(49150, ifAddr);

		std::cout << "available sockaddrs:" << std::endl;
		std::cout << SockaddrUtils::toString(ms.getAddrinfo()) << std::endl;

		auto ownSockaddr = SocketUtils::getsockname(ms.getSockfd());
		std::cout << "local sockaddr: " << ownSockaddr.toString() << std::endl;
		auto peerPort = static_cast<InetSocketAddress>(ownSockaddr).getPort();

		Inet4Address inetAddr(groupIpStr.c_str());

		ms.joinGroup(*inetAddr.get());

		auto receiveThread = runReceiveThread(ms);

		// sender

		// create multicast destination socket address
		InetSocketAddress socketAddress(inetAddr, peerPort);

		std::cout << "destination socketAddress: " << socketAddress.toString() << std::endl;

		sendMessages(ms, socketAddress);

		receiveThread.join();

		std::cout << "MulticastTest::test() done." << std::endl;

	}


};

}

#endif /* TESTSRC_MIKEJYG_MULTICASTTEST_H_ */
