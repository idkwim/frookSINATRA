EXTERN origKiSystemCall64:QWORD;
EXTERN myPrologKiSystemCall64:QWORD;

.data

.code

;Secure the registers and the stack :D
myKiSystemCall64ASM PROC
    swapgs ;USER->KERNEL
	mov qword ptr gs:[10h],rsp ;save stack pointer
	mov rsp,qword ptr gs:[1a8h] ;get stack pointer
	sub rsp,4096h ;far far away !
	
	;TODO: FPU registers
	mov [rsp+8],rax ;save register
	mov [rsp+16],rbx
	mov [rsp+24],rcx
	mov [rsp+32],rdx
	mov [rsp+40],rsi
	mov [rsp+48],rdi
    mov [rsp+56],r8
    mov [rsp+64],r9
    mov [rsp+72],r10
    mov [rsp+80],r11
    mov [rsp+88],r12
    mov [rsp+96],r13
    mov [rsp+104],r14
    mov [rsp+112],r15
    sub rsp, 120
	
	;TODO: poosh poosh ret !
	mov rax, qword ptr [myPrologKiSystemCall64];
	call rax;
	
	add	rsp, 120 
	mov rax,[rsp+8] ;restore register
	mov rbx,[rsp+16]
	mov rcx,[rsp+24]
	mov rdx,[rsp+32]
	mov rsi,[rsp+40]
	mov rdi,[rsp+48]
    mov r8,[rsp+56]
    mov r9,[rsp+64]
    mov r10,[rsp+72]
    mov r11,[rsp+80]
    mov r12,[rsp+88]
    mov r13,[rsp+96]
    mov r14,[rsp+104]
    mov r15,[rsp+112]
	
	;add rsp,4096h ; dont need this !
	mov rsp,qword ptr gs:[10h]
	swapgs ;KERNEL->USER
	
    jmp [origKiSystemCall64]
myKiSystemCall64ASM ENDP

END ; end of assembly file