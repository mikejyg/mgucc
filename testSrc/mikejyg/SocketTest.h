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

namespace mikejyg {

class SocketTest {
public:
	SocketTest();
	virtual ~SocketTest();

	/**
	 * a simple test with an echo server, send and receive a line.
	 */
	static void test(char const * hostname, unsigned port) {
		StreamSocket socket(hostname, port);

		std::string msg="hello\n";
		socket.send(msg.c_str(), msg.length());
		std::cout << "sent: " << msg << std::endl;

		SelectUtils::waitForRead(socket.getSockfd());

		char buf[100];
		auto k = socket.recv(buf, 100);

		std::string str(buf, k);
		std::cout << "received: " << str << std::endl;

	}


};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_SOCKETTEST_H_ */
