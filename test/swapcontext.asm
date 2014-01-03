swapcontext:
        /* Save the preserved registers, the registers used for passing args,
           and the return address.  */
        movq        %rbx, oRBX(%rdi)
        movq        %rbp, oRBP(%rdi)
        movq        %r12, oR12(%rdi)
        movq        %r13, oR13(%rdi)
        movq        %r14, oR14(%rdi)
        movq        %r15, oR15(%rdi)

        movq        %rdi, oRDI(%rdi)
        movq        %rsi, oRSI(%rdi)
        movq        %rdx, oRDX(%rdi)
        movq        %rcx, oRCX(%rdi)
        movq        %r8, oR8(%rdi)
        movq        %r9, oR9(%rdi)

        movq        (%rsp), %rcx
        movq        %rcx, oRIP(%rdi)
        leaq        8(%rsp), %rcx                /* Exclude the return address.  */
        movq        %rcx, oRSP(%rdi)

        /* We have separate floating-point register content memory on the
           stack.  We use the __fpregs_mem block in the context.  Set the
           links up correctly.  */
        leaq        oFPREGSMEM(%rdi), %rcx
        movq        %rcx, oFPREGS(%rdi)
        /* Save the floating-point environment.  */
        fnstenv        (%rcx)
        stmxcsr oMXCSR(%rdi)

        /* The syscall destroys some registers, save them.  */
        movq        %rsi, %r12

        /* Save the current signal mask and install the new one with
           rt_sigprocmask (SIG_BLOCK, newset, oldset,_NSIG/8).  */
        leaq        oSIGMASK(%rdi), %rdx
        leaq        oSIGMASK(%rsi), %rsi
        movl        $SIG_SETMASK, %edi
        movl        $_NSIG8,%r10d
        movl        $__NR_rt_sigprocmask, %eax
        syscall
        cmpq        $-4095, %rax                /* Check %rax for error.  */
        jae        SYSCALL_ERROR_LABEL        /* Jump to error handler if error.  */

        /* Restore destroyed registers.  */
        movq        %r12, %rsi

        /* Restore the floating-point context.  Not the registers, only the
           rest.  */
        movq        oFPREGS(%rsi), %rcx
        fldenv        (%rcx)
        ldmxcsr oMXCSR(%rsi)

        /* Load the new stack pointer and the preserved registers.  */
        movq        oRSP(%rsi), %rsp
        movq        oRBX(%rsi), %rbx
        movq        oRBP(%rsi), %rbp
        movq        oR12(%rsi), %r12
        movq        oR13(%rsi), %r13
        movq        oR14(%rsi), %r14
        movq        oR15(%rsi), %r15

        /* The following ret should return to the address set with
        getcontext.  Therefore push the address on the stack.  */
        movq        oRIP(%rsi), %rcx
        pushq        %rcx

        /* Setup registers used for passing args.  */
        movq        oRDI(%rsi), %rdi
        movq        oRDX(%rsi), %rdx
        movq        oRCX(%rsi), %rcx
        movq        oR8(%rsi), %r8
        movq        oR9(%rsi), %r9

        /* Setup finally  %rsi.  */
        movq        oRSI(%rsi), %rsi

        /* Clear rax to indicate success.  */
        xorl        %eax, %eax
        ret
        
