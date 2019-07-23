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
#include <thread>

namespace mikejyg {

class DatagramTest {
public:
	static const int MAX_DATAGRAM_SIZE = 65507;

public:
	DatagramTest();
	virtual ~DatagramTest();

	static void test(int port, int peerPort) {
		std::cout << "DatagramTest::test()..." << std::endl;

		int testRounds = 10;

		DatagramSocket datagramSocket(port, AF_INET);

		std::cout << "available sockaddrs:" << std::endl;
		std::cout << SockaddrUtils::toString(datagramSocket.getAddrinfo()) << std::endl;

		std::cout << "selected sockaddr: " << SockaddrUtils::toString(datagramSocket.getSelectedAddrinfo()) << std::endl;

		std::thread receiveThread([&](){
			int cnt=0;
			while (cnt < testRounds) {
				char buf[MAX_DATAGRAM_SIZE];
				DatagramPacket packet(buf, MAX_DATAGRAM_SIZE);
				datagramSocket.receive(packet);

				std::cout << "received: " << std::string(packet.getBuf(), packet.getLength());
				std::cout << ", from: " << packet.getSocketAddress().toString() << std::endl;

				cnt++;
			}
		});

		// sender

		InetSocketAddress socketAddress;
		socketAddress.setAddrinfoSelectFunction([](struct addrinfo const * res){
			return SockaddrUtils::selectAddrinfo(res, AF_INET);
		});
		socketAddress.init("localhost", peerPort);
		std::cout << "destination socketAddress: " << socketAddress.toString() << std::endl;

		int cnt=0;

		while (cnt < testRounds) {
			std::string msg("hello " + std::to_string(cnt++));
			DatagramPacket packet(msg.c_str(), msg.length(), socketAddress);
			datagramSocket.send(packet);
			std::cout << "sent: " << msg << ", to: " << packet.getSocketAddress().toString() << std::endl;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		receiveThread.join();

		std::cout << "DatagramTest::test() done." << std::endl;
	}

};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_DATAGRAMTEST_H_ */
