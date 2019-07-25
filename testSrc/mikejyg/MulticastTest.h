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

		MulticastSocket ms(49150, Inet4Address(interfaceIpStr.c_str()));

		std::cout << "local socket address: " << ms.getSocketAddress().toString() << std::endl;
		auto peerPort = static_cast<InetSocketAddress const &>(ms.getSocketAddress()).getPort();

		Inet4Address inetAddr(groupIpStr.c_str());

		ms.joinGroup(*inetAddr.get());

		auto receiveThread = runReceiveThread(ms);

		// sender

		// create multicast destination socket address
		InetSocketAddress socketAddress(inetAddr, peerPort);

		std::cout << "destination socket address: " << socketAddress.toString() << std::endl;

		sendMessages(ms, socketAddress);

		receiveThread.join();

		ms.close();

		std::cout << "MulticastTest::test() done." << std::endl;

	}


};

}

#endif /* TESTSRC_MIKEJYG_MULTICASTTEST_H_ */
