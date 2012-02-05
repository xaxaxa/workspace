/*
 * WTSPApplication.h
 *
 *  Created on: Nov 11, 2011
 *      Author: xaxaxa
 */

#ifndef WTSPAPPLICATION_H_
#define WTSPAPPLICATION_H_
#include <Wt/WApplication>
#include <string>
using namespace std;
using namespace Wt;
namespace WTSP {

class Application:public WApplication {
public:
	Application(const WEnvironment& env);
	virtual ~Application();
};

} /* namespace WTSP */
#endif /* WTSPAPPLICATION_H_ */
