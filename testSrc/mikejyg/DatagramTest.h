/*
 * DatagramTest.h
 *
 *  Created on: Jul 22, 2019
 *      Author: jgu
 */

#ifndef TESTSRC_MIKEJYG_DATAGRAMTEST_H_
#define TESTSRC_MIKEJYG_DATAGRAMTEST_H_

#include <mikejyg/DatagramSocket.h>
#include <iostream>
#include <mikejyg/SockaddrUtils.h>
#include <mikejyg/InetSocketAddress.h>
#include <mikejyg/OstreamBuilder.h>
#include <thread>
#include <mikejyg/SocketUtils.h>
#include <climits>

namespace mikejyg {

class DatagramTest {
public:
	static const int MAX_DATAGRAM_SIZE = 65507;

	static const int testRounds = 10;	// INT_MAX;

	//////////////////////////////////////////////

	static std::thread runReceiveThread(DatagramSocket & datagramSocket) {
		std::thread receiveThread([&](){
			int cnt=0;
			while (cnt < testRounds) {
				char buf[MAX_DATAGRAM_SIZE];
				DatagramPacket packet(buf, MAX_DATAGRAM_SIZE);
				datagramSocket.receive(packet);

				CoutBuilder::builder("received: ")->build(std::string(packet.getBuf(), packet.getLength()))
					.build(", from: ").build(packet.getSocketAddress().toString()).out();

				cnt++;
			}
		});

		return receiveThread;

	}

	static void sendMessages(DatagramSocket & datagramSocket, InetSocketAddress const & socketAddress) {
		int cnt=0;

		while (cnt < testRounds) {
			std::string msg("hello " + std::to_string(cnt++));
			DatagramPacket packet(msg.c_str(), msg.length(), socketAddress);
			datagramSocket.send(packet);

			CoutBuilder::builder("sent: ")->build(msg).build(", to: ").build(packet.getSocketAddress().toString()).out();

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

	}

public:
	DatagramTest() {}
	virtual ~DatagramTest() {}

	static void test(std::string const & interfaceIpStr) {
		std::cout << "DatagramTest::test()..." << std::endl;

		// wildcard address
//		DatagramSocket datagramSocket(AF_INET);

		Inet4Address ifAddr(interfaceIpStr.c_str());
		DatagramSocket datagramSocket(0, ifAddr);

		std::cout << "available sockaddrs:" << std::endl;
		std::cout << SockaddrUtils::toString(datagramSocket.getAddrinfo()) << std::endl;

		if (datagramSocket.getSelectedAddrinfo()!=nullptr)
			std::cout << "selected sockaddr: " << SockaddrUtils::toString( * datagramSocket.getSelectedAddrinfo() ) << std::endl;

		auto ownSockaddr = SocketUtils::getsockname(datagramSocket.getSockfd());
		std::cout << "local sockaddr: " << ownSockaddr.toString() << std::endl;
		auto peerPort = static_cast<InetSocketAddress>(ownSockaddr).getPort();

		auto receiveThread = runReceiveThread(datagramSocket);

		// sender

		InetSocketAddress socketAddress;
		socketAddress.setAddrinfoSelectFunction([](struct addrinfo const * res){
			return SockaddrUtils::selectAddrinfoByFamily(res, AF_INET);
		});
		socketAddress.init(interfaceIpStr, peerPort);
		std::cout << "destination socketAddress: " << socketAddress.toString() << std::endl;

		sendMessages(datagramSocket, socketAddress);

		receiveThread.join();

		std::cout << "DatagramTest::test() done." << std::endl;
	}

};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_DATAGRAMTEST_H_ */
