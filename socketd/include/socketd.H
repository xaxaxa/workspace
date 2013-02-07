/*
 * socketd.H
 *
 *  Created on: Feb 4, 2013
 *      Author: xaxaxa
 */

#ifndef SOCKETD_H_
#define SOCKETD_H_
#define SOCKETD_PROT_VERSION 1
#define SOCKETD_MAX_HEADERLEN 1024
namespace socketd
{
	struct protocolHeader
	{
		int version;
		enum
		{
			none = 0, handleConnection, ackConnection, shutdown
		} type;
		protocolHeader() :
				version(SOCKETD_PROT_VERSION), type(none) {
		}
	};
	struct prot_handleConnection
	{
		int64_t id;
		int32_t d;
		int32_t t;
		int32_t p;
		int32_t bufferLen;
	};
	struct prot_ackConnection
	{
		int64_t id;
		bool success;
	};
}

#endif /* SOCKETD_H_ */
