/*
 * WTSPApplication.h
 *
 *  Created on: Nov 11, 2011
 *      Author: xaxaxa
 */

#ifndef WTSPAPPLICATION_H_
#define WTSPAPPLICATION_H_
#include <Wt/WApplication>
using namespace Wt;
namespace WTSP {

class WTSPApplication:public WApplication {
public:
	WTSPApplication();
	virtual ~WTSPApplication();
};

} /* namespace WTSP */
#endif /* WTSPAPPLICATION_H_ */
