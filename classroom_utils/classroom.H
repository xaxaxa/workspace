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
#include <stdio.h>
#include <sys/wait.h>
using namespace std;
namespace classroom
{
	pid_t do_spawn(const char* path, char * const argv[])
	{
		pid_t pid = fork();
		if (pid == 0) //child
		{
			execv(path, argv);
			perror("exec"); //if this line executes, there's an error because a
			//					successful exec() does not return;
			_exit(0);
		}
		else if (pid > 0)
		{
			return pid;
		}
		else
			perror("fork");
		return -1;
	}

	//wait for a task to complete
	inline bool wait(pid_t pid)
	{
		if (pid == 0)
			return true;
		if (pid < 0)
			return false;
		int status;
		while (waitpid(pid, &status, 0) == -1)
			;
		return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
	}
	inline bool wait(vector<pid_t> pids)
	{
		size_t i;
		for (i = 0; i < pids.size(); i++)
			if (!wait(pids[i]))
				return false;
		return true;
	}
	struct client_config
	{
		string temp_dir; //has to end with "/"
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
	const char* daemon = "classroomd";
	struct client
	{
		string host;
		client_config cc;

		//{send:[local,other],recv:[local,other]}
		int sshpipe[2][2];

		client() :
				sshpipe(
				{ -1, -1 })
		{
		}
		pid_t copy_daemon()
		{
			const char* a[3] =
			{ "scp", daemon, (cc.ssh_username + "@" + host + ":" + cc.temp_dir + daemon).c_str() };
			return do_spawn("scp", (char* const *) a);
		}
		void create_pipe()
		{
			if (sshpipe[0][0] < 0)
			{
				if (pipe(sshpipe[0]) < 0)
					perror("pipe");
			}
			if (sshpipe[1][0] < 0)
			{
				if (pipe(sshpipe[1]) < 0)
					perror("pipe");
			}
		}
		void do_start_daemon()
		{
			create_pipe();
			int tmp = fork();
			if (tmp == 0) //child
			{
				dup2(sshpipe[0][1], 0);
				dup2(sshpipe[1][1], 1);
				const char* a[3] =
				{ "ssh", (cc.ssh_username + "@" + host).c_str(), (cc.temp_dir + daemon).c_str() };
				execv("ssh", (char* const *) a);
				perror("exec");
			}
			else if (tmp > 0)
			{
				return;
			}
			else
				perror("fork");
		}
	};
	struct config
	{
		string server_host;
		client_config cc;
		vector<client> clients;
	};

}

#endif /* CLASSROOM_H_ */
