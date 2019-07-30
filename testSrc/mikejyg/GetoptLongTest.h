/*
 * GetoptLongTest.h
 *
 *  Created on: Jul 26, 2019
 *      Author: jgu
 */

#ifndef TESTSRC_MIKEJYG_GETOPTLONGTEST_H_
#define TESTSRC_MIKEJYG_GETOPTLONGTEST_H_

#include <mikejyg/GetoptLong.h>
#include <iostream>

namespace mikejyg {

class GetoptLongTest {
public:
	GetoptLongTest();
	virtual ~GetoptLongTest();

	static void test(int argc, char * argv[]){
		GetoptLong getoptLong;

		// disable stderr output.
		getoptLong.setOpterr(0);

		getoptLong.addOption('h', "help", [&](){ std::cout << getoptLong.toHelpString(); }
			, "print help");
		getoptLong.addOption('s', "", [&](){ std::cout << "got -s" << std::endl; }
			, "short only");
		getoptLong.addOption(0, "longOnly", [&](){ std::cout << "got --longOnly" << std::endl; }
			, "long only");

		getoptLong.addOption<GetoptLong::OPTIONAL_ARGUMENT>('a', "", [&](const char * optarg){
			if (optarg)
				std::cout << "-a " << optarg << std::endl;
			else
				std::cout << "no argument provided for -a" << std::endl;
		}, "short option with optional argument", "optarg");

		try {
			// skip the unknow option, instead of quitting.
			getoptLong.parse(argc, argv, [](int optopt){
				std::cout << "unknown option: " << (char)optopt << " - skipped." << std::endl;
			});

		} catch (GetoptLong::NonoptionException & e) {
			// print out remaining unparsed non-option arguments, if any.
			std::cout << "non-option ARGV-elements: ";
			auto idx = getoptLong.getOptind();
			while (idx < argc)
				std::cout << argv[idx++] << " ";
			std::cout << std::endl;
		}

		std::cout << "GetoptLongTest::test() done." << std::endl;

	}

};

} /* namespace mikejyg */

#endif /* TESTSRC_MIKEJYG_GETOPTLONGTEST_H_ */
