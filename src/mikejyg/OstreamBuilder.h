/*
 * CoutBuilder.h
 *
 *      Author: jgu
 */

#ifndef MIKEJYG_OSTREAMBUILDER_H_
#define MIKEJYG_OSTREAMBUILDER_H_

#include <sstream>
#include <mutex>

namespace mikejyg {

/**
 * to provide synchronized output to std::cout, or other ostream.
 */
template<std::ostream * outp>
class OstreamBuilder {
protected:
	std::stringstream ss;

	OstreamBuilder() {}

public:

	virtual ~OstreamBuilder() {}

//	OstreamBuilder(OstreamBuilder && b2) : ss( std::move(b2.ss) ) {}
//
//	OstreamBuilder & operator = (OstreamBuilder && b2) {
//		ss = std::move(b2.ss);
//		return *this;
//	}

	template<typename T>
	static std::unique_ptr<OstreamBuilder> builder(T const & v) {
		auto builder = new OstreamBuilder;
		builder->ss << v;
		return std::unique_ptr<OstreamBuilder>(builder);
	}

	template<typename T>
	OstreamBuilder & build(T const & t) {
		ss << t;
		return *this;
	}

	void out() {
		static std::mutex mutex;

		{
			std::unique_lock<std::mutex> lock(mutex);
			*outp << ss.str() << std::endl;
		}
		ss.clear();
	}

};

typedef OstreamBuilder<&std::cout> CoutBuilder;


} /* namespace mikejyg */

#endif /* MIKEJYG_OSTREAMBUILDER_H_ */
