/*
 * classroom.H
 *
 *  Created on: Feb 24, 2012
 *      Author: xaxaxa
 */

#ifndef CLASSROOM_H_
#define CLASSROOM_H_

#include <string>
#include <vector>
using namespace std;
namespace classroom
{
	pid_t do_spawn(const char* path, char *const argv[])
	{
		pid_t pid=fork();
		if(pid==0)
		{
			execv(path,argv);
			perror("exec");
		}
		else if(pid>0)
		{
			return pid;
		}
		else perror("fork");
		return -1;
	}
	struct client_config
	{
		string temp_dir;	//has to end with "/"
		string ssh_username;
		int unprivileged_uid; //only one of unprivileged_uid and unprivileged_user
		string unprivileged_user; //needs to be set. when unprivileged_uid=0, it will
								  //resolve unprivileged_user into a uid
		int unprivileged_gid;
		string unprivileged_group;
		client_config() :
				temp_dir("/tmp/classroomd/"), ssh_username("root"), unprivileged_uid(65533),
						unprivileged_gid(65533)
		{
		}
	};
	struct client
	{
		string host;
		client_config cc;
	};
	struct config
	{
		string server_host;
		client_config cc;
		vector<client> clients;
	};
	pid_t copy_daemon(const client& c)
	{
		const char* daemon = "classroomd";
		return do_spawn("scp",{"scp",daemon,(c.cc.ssh_username+"@"+c.host+":"+c.cc.temp_dir+daemon).c_str()});
	}
}

#endif /* CLASSROOM_H_ */
