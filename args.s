# args.s
.text
.globl _start
        
_start:
        pop		%rcx		# argc
vnext:
        pop		%rcx		# argv
        test 	%rcx, %rcx  # 空指针表明结束
        jz		exit
        mov		%rcx, %rbx
        xor		%edx, %edx
strlen:
        mov		(%rbx), %al
        inc		%edx
        inc		%rbx
        test	%al, %al
        jnz		strlen
        movl	$10, -1(%rbx)
        mov		$4, %rax        # 系统调用号(sys_write) 
        mov		$1, %rbx        # 文件描述符(stdout) 
        int		$0x80
        jmp		vnext
exit:
        mov		$1,%rax         # 系统调用号(sys_exit) 
        xor		%rbx, %rbx      # 退出代码
        int 	$0x80
		
        ret
