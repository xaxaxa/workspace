/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */


#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <exception>
using namespace std;

struct SSH
{
	//pipes
	int32_t stdin;
	int32_t stdout;
	int32_t stderr;
	//process id
	pid_t pid;
	void kill() {
		if(pid>0) ::kill(pid, 9);
	}
	void send(const char* data, int len) {
		write(stdin, data, len);
	}
	void send(const char* data) {
		send(data, strlen(data));
	}
};
template<class T> inline T checkError(T asdfg) {
	if(asdfg<0) throw runtime_error(strerror(errno));
	return asdfg;
}
//cmd[0] is the command name, cmd[1] is argv[0] of child
bool SSHConnect(SSH& ssh, const vector<string> cmd) {
	char* cmds[cmd.size()+1];
	for(size_t i=0;i<cmd.size();i++) {
		cmds[i]=(char*)cmd[i].c_str();
	}
	cmds[cmd.size()]=NULL;
	int32_t I[2];
	int32_t O[2];
	int32_t E[2];
	checkError(pipe(I));
	checkError(pipe(O));
	checkError(pipe(E));
	
	pid_t tmp=fork();
	if(tmp>0) {
		close(I[0]);
		close(O[1]);
		close(E[1]);
		ssh.pid=tmp;
		ssh.stdin=I[1];
		ssh.stdout=O[0];
		ssh.stderr=E[0];
		return true;
	} else if(tmp==0) {
		close(I[1]);
		close(O[0]);
		close(E[0]);
		dup2(I[0],0);
		dup2(O[1],1);
		dup2(E[1],2);
		close(I[0]);
		close(O[1]);
		close(E[1]);
		execvp(cmds[0], cmds+1);
		_exit(1);
	} else {
		return false;
	}
}
int main(int argc, char** argv)
{
	SSH ssh;
	if(!SSHConnect(ssh, {"ssh","ssh","root@10.5.0.1","bash"})) {
		printf("error\n");
	}
	ssh.send("touch aaaaaaaaa");
	ssh.send("mkdir aaaaaaaa");
}
