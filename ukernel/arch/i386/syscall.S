//CURRENTLY NOT USED SINCE GLIBC IS USABLE (save/restore %gs)
.globl do_syscall,syscall1,syscall2

do_syscall:
	int $0x80
	ret

syscall1:		// for syscalls with 1 argument
	push %ebx
	mov 0x8(%esp), %eax
	mov 0xc(%esp), %ebx
	call do_syscall
	pop %ebx
	ret

syscall2:		// for syscalls with 2 arguments
	push %ebx
	mov 0x8(%esp), %eax
	mov 0xc(%esp), %ebx
	mov 0x10(%esp), %ecx
	call do_syscall
	pop %ebx
	ret

syscall3:
	push %ebx
	mov 0x8(%esp), %eax
	mov 0xc(%esp), %ebx
	mov 0x10(%esp), %ecx
	mov 0x14(%esp), %edx
	call do_syscall
	pop %ebx
	ret
