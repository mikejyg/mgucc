/*
 * GetoptLong.h
 *
 *  Created on: Jul 26, 2019
 *      Author: jgu
 */

#ifndef MIKEJYG_GETOPTLONG_H_
#define MIKEJYG_GETOPTLONG_H_

#include <getopt.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <cstring>

namespace mikejyg {

/**
 * a more user friendly class for getopt_long.
 */
class GetoptLong {
public:
	enum ArgumentRequirement : int {
		NO_ARGUMENT = no_argument,
		REQUIRED_ARGUMENT = required_argument,
		OPTIONAL_ARGUMENT = optional_argument
	};

	class UnknownOptionException : public std::exception {};

protected:
	struct OptionStruct {
		char optChar;
		std::string longOptStr;

		ArgumentRequirement argReq;
		std::string optargName;		// the name of the argument to the option

		std::function< void(const char * arg)> action;

		std::string description;

		OptionStruct(char optChar, std::string const & longOptStr, ArgumentRequirement argReq)
			: optChar(optChar), longOptStr(longOptStr), argReq(argReq)
		{}
	};

	// store the OptionStructs
	std::vector<OptionStruct> options;

	/**
	 * option value to OptionStruct map (index to options)
	 * the value can be optChar, or a synthetic value for long option.
	 */
	std::map<int, unsigned> optValToOptionMap;

	// synthesize the val for long option.
	int longOptVal;

	// for the short option argument
	std::string optstring;

	// for the long option argument
	std::unique_ptr<struct option> long_options;

	int longOptCnt;

	// working variables
	int longIndex;

	//////////////////////////////////////////////

	void buildLongOptions() {
		long_options.reset(new struct option[ longOptCnt + 1 ]);
		int idx=0;
		for (auto & mapPair : optValToOptionMap) {
			auto & optionStruct = options[mapPair.second];
			if (optionStruct.longOptStr.empty())
				continue;

			auto & structOption = long_options.get()[idx++];

			structOption.name = optionStruct.longOptStr.c_str();
			structOption.has_arg = optionStruct.argReq;
			structOption.flag = nullptr;
			structOption.val = mapPair.first;
		}
		memset( long_options.get() + idx, 0, sizeof(struct option));
	}

	void buildOptstring() {
		for (auto & optionStruct : options) {
			if (optionStruct.optChar!=0) {
				optstring += optionStruct.optChar;
				if (optionStruct.argReq==REQUIRED_ARGUMENT)
					optstring += ":";
				else if (optionStruct.argReq==OPTIONAL_ARGUMENT)
					optstring += "::";
			}
		}
	}

	template<ArgumentRequirement argReq>
	class SetActionHelper;

public:
	GetoptLong() : longOptVal(256), longOptCnt(0) {}

	/**
	 * If optChar is 0, then no short option.
	 * If longOptStr is empty, then no long option
	 */
	template<ArgumentRequirement argReq=NO_ARGUMENT, typename F>
	OptionStruct & addOption(char optChar, std::string const & longOptStr
			, F && f, std::string const & description
			, std::string const & optargName="") {
		options.emplace_back(OptionStruct(optChar, longOptStr, argReq));

		auto & optionStruct = options.back();

		optionStruct.description = description;
		optionStruct.optargName = optargName;

		SetActionHelper<argReq>::setAction(optionStruct, std::forward<F>(f));

		if (optChar!=0)
			optValToOptionMap[optChar] = options.size()-1;
		else if (!longOptStr.empty()) {
			optValToOptionMap[longOptVal++] = options.size()-1;
		}

		if (!longOptStr.empty())
			longOptCnt++;

		return optionStruct;

	}

	/**
	 * f is the unknown option handler, void(int)
	 */
	template<typename F>
	void parse( int argc, char *argv[], F && f) {
		optind=1;

		buildLongOptions();
		buildOptstring();

		while (true) {

			auto c = getopt_long(argc, argv, optstring.c_str(), long_options.get(), &longIndex);

			if (c == -1)	// all command-line options have been parsed
				break;

			if (c=='?') {
				f(optopt);
				continue;
			}

			auto it = optValToOptionMap.find(c);

			if (it==optValToOptionMap.end())
				throw std::runtime_error("getopt_long() return not found in map: " + std::to_string(c));

			auto & optionStruct = options[it->second];

			optionStruct.action(optarg);
		}
	}

	/**
	 * with default unknown option handler, throws an exception.
	 */
	void parse( int argc, char *argv[]) {
		parse(argc, argv, [](int){ throw UnknownOptionException(); });
	}

	/**
	 * the current index into the provided argv.
	 */
	static int getOptind() {
		return optind;
	}

	/**
	 * the current option character.
	 */
	static char getOptopt() {
		return optopt;
	}

	/**
	 * set to 0 to suppress stderr output.
	 */
	static void setOpterr(int opterr) {
		::opterr = opterr;
	}

	std::string toHelpString() const {
		std::string str;
		for (auto & optionStruct : options) {
			if ( optionStruct.optChar != 0 ) {
				str += "-";
				str += optionStruct.optChar;
			}

			if ( ! optionStruct.longOptStr.empty() ) {
				str += ", --" + optionStruct.longOptStr;
			}

			if ( ! optionStruct.optargName.empty() )
				str += " " + optionStruct.optargName;

			str += '\t' + optionStruct.description + '\n';

		}

		return str;
	}


};

template<>
class GetoptLong::SetActionHelper<GetoptLong::NO_ARGUMENT> {
public:
	template<typename F>
	static void setAction(OptionStruct & optionStruct, F && f) {
		optionStruct.action = [f](const char * arg){f();};
	}
};

template<>
class GetoptLong::SetActionHelper<GetoptLong::REQUIRED_ARGUMENT> {
public:
	template<typename F>
	static void setAction(OptionStruct & optionStruct, F && f) {
		optionStruct.action = std::forward<F>(f);
	}
};

template<>
class GetoptLong::SetActionHelper<GetoptLong::OPTIONAL_ARGUMENT> {
public:
	template<typename F>
	static void setAction(OptionStruct & optionStruct, F && f) {
		optionStruct.action = std::forward<F>(f);
	}
};


} /* namespace mikejyg */

#endif /* MIKEJYG_GETOPTLONG_H_ */
