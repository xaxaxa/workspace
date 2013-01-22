/*
 * config.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */
#include "config.hpp"

namespace config
{
	configmanager* configmanager::getmainconfig()
	{
		if (mainconfig == NULL)
		{
			mainconfig = new configmanager();
			mainconfig->load();
		}
		return mainconfig;
	}
	void configmanager::load()
	{

	}

	rtconfigmanager* rtconfigmanager::getmainconfig()
	{
		if (rtmainconfig == NULL)
		{
			rtmainconfig = new rtconfigmanager();
			rtmainconfig->load(configmanager::getmainconfig());
		}
		return rtmainconfig;
	}
	void rtconfigmanager::load(configmanager* c)
	{

	}
}
