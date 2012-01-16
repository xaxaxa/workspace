/*
 * procattach.C
 * 
 * Copyright 2012 Unknown <s-wang_cary@client-101>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>

using namespace std;
void injected_func(void** start, void** end)
{
	*start=&&s;
	*end=&&e;
	return;
	s:;
	//code to be injected
	system("echo injected");
	e:;
}
#define WORDSIZE 8
typedef unsigned long int PTR;
int main(int argc, char **argv)
{
	if(argc<2)
	{
		cout << "Usage: " << argv[0] << " pid" << endl;
		return 1;
	}
	pid_t pid;
	pid=atoi(argv[1]);
	ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    waitpid(pid,NULL,0);
    
    user_regs_struct r;
    ptrace(PTRACE_GETREGS, pid, NULL, &r);
    
    //code to be injected:
    cout << "eip: " << r.rip << endl;
    PTR *start,*end;
    injected_func((void**)&start,(void**)&end);
    
    int i;
    int l=((PTR)end-(PTR)start)/WORDSIZE;
    for(i=0;i<l;i++)
    {
		ptrace(PTRACE_POKEDATA, pid, r.rip + (i * WORDSIZE), start[i]);
	}
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
	return 0;
}

