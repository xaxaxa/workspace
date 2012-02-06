/*
 * Parser.H
 *
 *  Created on: Feb 5, 2012
 *      Author: xaxaxa
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;
namespace WTSP
{

	class Parser
	{
	public:
		Parser();
		void ParsePage(StreamReader& in, StreamWriter& out)
		{
			StringBuilder tmp;
			//scan for "<%"
			STRING delim[1]{"<%"};
			while(true)
			{
				Int br=in.Read(tmp,delim,1,NULL);
				if(br<0)break;
				if(br>0)
				{

				}
			}
		}
		virtual ~Parser();
	};

} /* namespace WTSP */
#endif /* PARSER_H_ */
