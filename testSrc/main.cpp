/*
 * main.cpp
 *
 * Author: mikejyg
 */

#include <mikejyg/IntUtils.h>
#include "mikejyg/SockaddrTest.h"
#include "mikejyg/SocketTest.h"

using namespace mikejyg;

int main(int argc, char **argv) {
	IntUtils::test();

	SockaddrTest::test();

	// args: hostname, port
	SocketTest::test(argv[1], std::stoi(argv[2]));

}
