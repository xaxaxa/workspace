/*
 * config.hpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <asyncsock.hpp>
namespace config
{
	struct listen
	{
	public:
		xaxaxa::Sockets::EndPoint *ep;
		int backlog;
	};
	struct binding
	{
	public:
		xaxaxa::Sockets::EndPoint *ep;
		string mountpoint;
	};
	struct vhost
	{
	public:
		binding bindings[];
		int bindings_c;
		string exepath;
		int requestsPerProcess;
		int maxRequestsPerProcess;
		int maxProcesses;
	};
	struct rtvhost: public vhost //runtime config
	{
	public:
		int processes[];
		int processCount;
	};
	class configmanager
	{
	public:
		listen listens[];
		int listens_c;
		vhost vhosts[];
		int vhosts_c;
		void load();

		static configmanager* getmainconfig();
	};
	static configmanager* mainconfig;
	class rtconfigmanager
	{
	public:
		listen listens[];
		int listens_c;
		rtvhost vhosts[];
		int vhosts_c;
		void load(configmanager *c);

		static rtconfigmanager* getmainconfig();
	};
	static rtconfigmanager* rtmainconfig;
}

#endif /* CONFIG_HPP_ */
