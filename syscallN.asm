; Here @_start symbol (program entry point) is defined:
; * init libc
; * call @main
; * do exit syscall

; int syscallN (int n, ...) - do Linux syscall n with N args:

; * Load syscall number @n from stack to EAX
; * Load N args from stack to appropriate regs (EBX - arg1, ECX - arg2, 
;     EDX - arg3, ESI - arg4, EDI - arg5, EBP - arg6)
; * Exec int 0x80
; * Examine return value in EAX: if EAX>=0 - OK, return it
; * If EAX<0 - set @errno to -EAX and return -1

; NOTE: the most common/useful syscalls have 0-3 args

; NOTE: int 0x80 preserves all regs during syscall

; C calling convention:
; * All args are in stack in backwards order: first is pushed last
; * Function can clobber only EAX, ECX, EDX
; * Return value in EAX
; * Caller pops args from stack 

format ELF

public	syscall0
public	syscall1
public	syscall2
public	syscall3
public	syscall4
public	syscall5
public	_start
public errno
extrn	main
extrn	_init
extrn	exit

section ".text" executable 

_start:

; Stack layout:

; ESP 	-> argc
; ESP+4 -> argv[0]
; ESP+8 -> argv[1]
; ...
; ...   -> 0
; ...   -> envp[0]
; ...   -> envp[1]
; ...
; ...  -> 0
  
	pop	eax		; argc
	mov	ecx, esp	; argv
	lea	edx, [ecx + eax * 4 + 4]	; envp
	push	edx
	push	ecx
	push	eax
	call	_init
	call	main
;	mov	ebx, eax	; Return value of main
;	mov	eax, 1		; Exit syscall
;	int	0x80		; Do it!
	push	eax
	call	exit

syscall0:
	push	ebx		; Just to be compatible with the rest
	push	esi		; Just to be compatible with the rest
	mov	eax, [esp + 12]
	jmp	do_sys

syscall1:
	push	ebx
	push	esi
	mov	eax, [esp + 12]
	mov	ebx, [esp + 16]
	jmp	do_sys

syscall2:
	push	ebx
	push	esi
	mov	eax, [esp + 12]
	mov	ebx, [esp + 16]
	mov	ecx, [esp + 20]
	jmp	do_sys

syscall3:
	push	ebx
	push	esi
	mov	eax, [esp + 12]
	mov	ebx, [esp + 16]
	mov	ecx, [esp + 20]
	mov	edx, [esp + 24]
do_sys:	int	0x80
	cmp	eax, 0
	jge	.m1
	neg	eax
	mov	[errno], eax
	mov	eax, -1
.m1:	pop	esi
	pop	ebx
	ret

syscall4:
	push	ebx
	push	esi
	mov	eax, [esp + 12]
	mov	ebx, [esp + 16]
	mov	ecx, [esp + 20]
	mov	edx, [esp + 24]
	mov	esi, [esp + 28]
	jmp	do_sys

syscall5:
	push	ebx
	push	esi
	push	edi
	mov	eax, [esp + 16]
	mov	ebx, [esp + 20]
	mov	ecx, [esp + 24]
	mov	edx, [esp + 28]
	mov	esi, [esp + 32]
	mov	edi, [esp + 36]
	int	0x80
	cmp	eax, 0
	jge	.m1
	neg	eax
	mov	[errno], eax
	mov	eax, -1
.m1:	pop	edi
	pop	esi
	pop	ebx
	ret

section ".data" writable

errno:	dd	-1

