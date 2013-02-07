/*
 * statemachines.H
 *
 *  Created on: Feb 1, 2013
 *      Author: xaxaxa
 */

#ifndef STATEMACHINES_H_
#define STATEMACHINES_H_
#include <functional>
#include <stdint.h>
#include <string>
using namespace std;
namespace CP {
	//streamReader C-style API/ABI; this is to avoid breaking the ABI when the
	//internal state machine structure changes
	//usage example:
	//streamReader* sr=(streamReader*)malloc(streamReader_getSize()+4096);
	//streamReader_init(sr, 4096);
	// ...
	//streamReader_deinit(sr);
	struct streamReader;
	int streamReader_getSize();
	void streamReader_init(streamReader* sr, int capacity);
	void streamReader_deinit(streamReader* sr);
	tuple<uint8_t*, int> streamReader_beginPutData(streamReader* sr);
	void streamReader_endPutData(streamReader* sr, int len);
	void streamReader_readUntilString(streamReader* sr, string delim);
	void streamReader_readUntilChar(streamReader* sr, char delim);
	void streamReader_setCallback(streamReader* sr, const function<void(uint8_t*, int, bool)>& cb);
	tuple<uint8_t*, int> streamReader_getBufferData(streamReader* sr);
	void streamReader_reset(streamReader* sr);
}


#endif /* STATEMACHINES_H_ */
