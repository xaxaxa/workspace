/*
 * configparser.H
 *
 *  Created on: Mar 2, 2013
 *      Author: xaxaxa
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_
#include "socketd_internal.H"

namespace socketd
{
	class ParserException: public std::exception
	{
	public:
		string message;
		int32_t number;
		ParserException();
		ParserException(int32_t number);
		ParserException(string message, int32_t number = 0);
		~ParserException() throw ();
		const char* what() const throw ();
	};
	void loadConfig(const char* conf, int len, socketd& sd);
	void reloadConfig(const char* conf, int len, socketd& sd);
}

#endif /* CONFIGPARSER_H_ */
