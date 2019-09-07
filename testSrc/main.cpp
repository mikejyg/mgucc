/*
 * main.cpp
 *
 * Author: mikejyg
 */

#include <mikejyg/IntUtils.h>
#include <mikejyg/OstreamBuilder.h>
#include "mikejyg/GetoptLongTest.h"

using namespace mikejyg;
using namespace std;

void printHelp() {
	cout << "usages: mgucc command" << endl;
	cout << "\tgetoptTest\trun GetoptLongTest." << endl;
	cout << "\tintUtilsTest\trun IntUtils tests." << endl;
}

int main(int argc, char **argv) {
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

	} else {
		std::cout << "unknown command: " << cmdStr << std::endl;
		printHelp();
		exit(1);
	}


}

