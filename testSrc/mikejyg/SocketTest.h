/*
 * SocketTest.h
 *
 *  Created on: Jul 21, 2019
 *      Author: mikejyg
 */

#ifndef TESTSRC_MIKEJYG_SOCKETTEST_H_
#define TESTSRC_MIKEJYG_SOCKETTEST_H_

#include <mikejyg/StreamSocket.h>
#include <mikejyg/SelectUtils.h>
#include <iostream>
#include <mikejyg/ServerSocket.h>
#include <thread>

namespace mikejyg {

class SocketTest {
public:
	SocketTest();
	virtual ~SocketTest();

	/**
	 * a simple test with an echo server, send and receive a line.
	 */
	static void testClient(char const * hostname, unsigned port) {
		std::cout << "testClient() " << hostname << " " << port << "..." << std::endl;

		StreamSocket socket(hostname, port);

		std::cout << "available addrinfos:" << std::endl;
		std::cout << SockaddrUtils::toString( socket.getAddrinfo() ) << std::endl;

		std::cout << "peer socket: " << socket.getSocketAddress().toString() << std::endl;

		socket.connect();

		int cnt=0;

		while (true) {
			std::string msg="hello " + std::to_string(++cnt) + "\n";
			socket.send(msg.c_str(), msg.length());
			std::cout << "sent: " << msg << std::endl;

			SelectUtils::waitForRead(socket.getSockfd());

			char buf[100];
			auto k = socket.recv(buf, 100);

			std::string str(buf, k);
			std::cout << "received: " << str << std::endl;

			std::this_thread::sleep_for(std::chrono::seconds(3));
		}

		std::cout << "testClient() done." << std::endl;
	}

	static void serve(std::tuple<StreamSocket, SocketAddress> & client) {
		char buf[100];

		std::cout << "client: " << std::get<1>(client).toString() << std::endl;

		while (true) {
			auto k = std::get<0>(client).recv(buf, 100);
			if (k==0) {
				std::cout << "client performed orderly shutdown." << std::endl;
				break;
			}

			std::cout << "received: " << std::string(buf, k) << std::endl;
			std::get<0>(client).send(buf, k);
			std::cout << "sent response." << std::endl;
		}

	}

	static void testServer(unsigned port) {
		std::cout << "testServer() " << port << "..." << std::endl;

		ServerSocket serverSocket(port);

		std::cout << "available addrinfos:" << std::endl;
		std::cout << SockaddrUtils::toString( serverSocket.getAddrinfo() ) << std::endl;

		std::cout << "server socket: " << serverSocket.getSocketAddress().toString() << std::endl;

		serverSocket.listen();

		while (true) {
			std::cout << "accepting connections..." << std::endl;
			auto client = serverSocket.accept();

			serve(client);

		}

	}


};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_SOCKETTEST_H_ */
