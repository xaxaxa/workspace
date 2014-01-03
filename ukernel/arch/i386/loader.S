//CURRENTLY NOT USED SINCE GLIBC IS USABLE (save/restore %gs)

.globl _start

_start:
	movl %esp, %eax
	addl $0x4, %eax
	push %eax
	push 0x4(%esp)
	call main
	movl %eax,%ebx
	movl $1,%eax
	int $0x80

