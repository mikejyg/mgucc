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

	static void test(std::string const & listeningIpStr, std::string const & destHost) {
		std::cout << "DatagramTest::test()..." << std::endl;

		DatagramSocket datagramSocket(0, Inet4Address(listeningIpStr.c_str()));

		std::cout << "local socket address: " << datagramSocket.getSocketAddress().toString() << std::endl;
		auto peerPort = static_cast<InetSocketAddress const &>(datagramSocket.getSocketAddress()).getPort();

		auto receiveThread = runReceiveThread(datagramSocket);

		// sender

		InetSocketAddress socketAddress(destHost, peerPort, [](struct addrinfo const * res){
			return AddrinfoUtils::selectAddrinfoByFamily(res, AF_INET);
		});
		std::cout << "destination socket address: " << socketAddress.toString() << std::endl;

		sendMessages(datagramSocket, socketAddress);

		receiveThread.join();

		datagramSocket.close();

		std::cout << "DatagramTest::test() done." << std::endl;
	}

};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_DATAGRAMTEST_H_ */
