/*
 * main.cpp
 *
 * Author: mikejyg
 */

#ifdef _WIN32
#include <mikejyg/WsaService.h>
#endif

#include <mikejyg/IntUtils.h>
#include "mikejyg/SockaddrTest.h"
#include "mikejyg/SocketTest.h"
#include "mikejyg/AdapterInfo.h"
#include <mikejyg/OstreamBuilder.h>
#include "mikejyg/GetoptLongTest.h"

using namespace mikejyg;
using namespace std;

void printHelp() {
	cout << "usages: mgucc command" << endl;
	cout << "\tgetoptTest\trun GetoptLongTest." << endl;
	cout << "\tintUtilsTest\trun IntUtils tests." << endl;
	cout << "\tadapterInfoTest\trun AdapterInfo tests (Windows only)." << endl;
	cout << "\tsockaddrTest\trun SockaddrTest." << endl;
	cout << "\tsocketTest\trun SocketTest." << endl;
}

int main(int argc, char **argv) {
#ifdef _WIN32
	WsaService::getInstance();
#endif

	if (argc<2) {
		std::cout << "missing command argument." << std::endl;
		printHelp();
		exit(1);
	}

	int argIdx=1;
	std::string cmdStr = argv[argIdx];

	if (cmdStr=="getoptTest") {
		GetoptLongTest::test(argc-argIdx, argv+argIdx);

	} else if (cmdStr=="intUtilsTest") {
		IntUtils::test();

	} else if (cmdStr=="sockaddrTest") {
		SockaddrTest::test(argc-argIdx, argv+argIdx);

#ifdef _WIN32
	} else if (cmdStr=="adapterInfoTest") {
		std::cout << "AdapterInfo..." << std::endl;
		AdapterInfo::ipconfig();
		AdapterInfo::getAdaptersAddresses();

#endif
	} else if (cmdStr=="socketTest") {
		SocketTest socketTest;
		socketTest.test(argc-argIdx, argv+argIdx);

	} else {
		std::cout << "unknown command: " << cmdStr << std::endl;
		printHelp();
		exit(1);
	}


}

