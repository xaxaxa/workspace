.global _handle_syscall,_set_esp_and_jmp,_call_child_process,_call_child_process_returnto
_handle_syscall:
	//save gs
	push %gs
	//save all registers
	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	//get the kernel's gs segment
	call _get_kthread_gs

	//switch gs
	mov %eax, %gs

	//call syscall handler
	call do_handle_syscall

	//restore registers and revert stack (28 bytes)
	mov 0x04(%esp), %ebx
	mov 0x08(%esp), %ecx
	mov 0x0C(%esp), %edx
	mov 0x10(%esp), %esi
	mov 0x14(%esp), %edi
	mov 0x18(%esp), %ebp

	//revert stack, leaving only saved %gs on stack
	add $0x1C, %esp

	//restore gs
	pop %gs

	//return
	ret
_set_esp_and_jmp:
	mov 0x8(%esp), %eax
	mov 0x4(%esp), %esp
	jmp *%eax

//void (void* new_stack, void* entry, void** put_sp)
_call_child_process:
	mov 0x0C(%esp), %edx	//edx=put_sp

	mov 0x04(%esp), %eax	//new stack
	mov 0x08(%esp), %ecx	//entry
	//push all registers onto old stack
	pusha
	//save stack pointer
	mov %esp, (%edx)		//*edx=esp
	mov %eax, %esp
	jmp *%ecx
_call_child_process_returnto:
	popa
	ret
