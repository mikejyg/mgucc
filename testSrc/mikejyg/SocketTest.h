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
#include <vector>
#include <map>
#include <mikejyg/ThreadPool.h>
#include <mutex>

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

		Inet4Address inet4Address;
		inet4Address.initFromHostname(hostname);

		StreamSocket socket(inet4Address, port);
		std::cout << "local socket address: " << socket.getSocketAddress().toString() << std::endl;
		std::cout << "peer socket address: " << socket.getPeerSocketAddress().toString() << std::endl;

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

		socket.close();

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

		std::get<0>(client).close();
	}

	static void testServer(unsigned port) {
		std::cout << "testServer() " << port << "..." << std::endl;

		ServerSocket serverSocket(port, Inet4Address("0.0.0.0"));

		std::cout << "server socket address: " << serverSocket.getSocketAddress().toString() << std::endl;

		serverSocket.listen();

		ThreadPool threadPool(2);

		int clientCnt=0;

		std::vector<std::thread> serveThreads;
		std::map< int, std::tuple<StreamSocket, SocketAddress> > clients;

		std::mutex clientsMutex;

		while (true) {
			std::cout << "accepting connections..." << std::endl;

			auto client = serverSocket.accept();

			{
				std::unique_lock<std::mutex> lock(clientsMutex);
				clients.emplace( clientCnt, std::move(client) );
			}

			threadPool.enqueue([ & clientsMutex, & clients, clientCnt]{
				serve( clients.find(clientCnt)->second );

				{
					std::unique_lock<std::mutex> lock(clientsMutex);
					clients.erase(clientCnt);
				}
				std::cout << "client removed: " << clientCnt << std::endl;

			});

			std::cout << "client added: " << clientCnt << std::endl;
			clientCnt++;

		}

		serverSocket.close();
	}

};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_SOCKETTEST_H_ */
