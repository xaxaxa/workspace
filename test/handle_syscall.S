.global _handle_syscall,_set_esp_and_jmp
_handle_syscall:
	//save eax
	push %eax
	
	//save gs
	mov %gs, %eax
	push %eax
	
	//switch gs
	mov $0x63, %eax
	mov %eax, %gs
	
	//restore eax
	mov 0x04(%esp), %eax
	//push arguments
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	//call syscall handler
	call do_handle_syscall
	
	//restore registers and revert stack (24 bytes)
	add $0x4,%esp
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	
	//put return value on the stack, overwriting
	//previous saved eax
	mov %eax, 0x04(%esp)
	
	//restore gs
	pop %eax
	mov %eax, %gs
	//return
	pop %eax
	ret

_set_esp_and_jmp:
	pop %eax
	pop %eax
	pop %ebx
	mov %eax, %esp
	jmp *%ebx
