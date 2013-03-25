/*
 * iptsocks.H
 *
 *  Created on: Mar 23, 2013
 *      Author: xaxaxa
 */

#ifndef IPTSOCKS_H_
#define IPTSOCKS_H_


#define WARNLEVEL 5
#define WARN(LEVEL,MSG) if(LEVEL<=WARNLEVEL){if(LEVEL<=1)cerr << "\x1B[41;1;33m"; else if(LEVEL<=2)cerr << "\x1B[1;1;1m"; cerr << MSG << "\x1B[0;0;0m" << endl;}


#endif /* IPTSOCKS_H_ */
