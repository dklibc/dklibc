; Fasm source of string functions

format ELF

section ".text" executable

public strlen
public strcmp
public strcpy
public strchr
public memset
public memcpy
public memcmp

; int strlen (char *s)
; Compute string length
; Return: string length ('\0' at the string end isn't counted)
strlen:
	push	edi
	
	xor	al, al
	mov	ecx, 0xffffffff
	mov	edi, [esp + 8]
	
	cld
	repne	scasb
	
	; ecx = 0xffffffff - (len + 1)
	; len = 0xffffffff - ecx - 1
	; len = 0 - 1 - ecx - 1
	; -x = ~x + 1
	; len = -1 + ~ecx + 1 - 1
	; len = ~ecx - 1
	not	ecx
	dec	ecx
	mov	eax, ecx

	pop	edi
	ret

; int strcmp (char *s, char *q)
; Compare strings
; Return: 0 - they are equal, -1 - s < q, 1 - s > q 
strcmp:
	push	esi
	push	edi

	mov	esi, [esp + 12]	; @s	
	mov	edi, [esp + 16]	; @q
	cld

.m3:	lodsb
	scasb
	jne	.m1
	test	al, al
	jnz	.m3

	xor	eax, eax
	jmp	.m5

.m1:	mov	eax, 1
	ja	.m5
	neg	eax	

.m5:	pop	edi
	pop	esi
	ret

; char *strcpy (char *to, char *from)
; Copy string @from to buffer @to
; Return: @to
strcpy:
	push	esi
	push	edi
	
	mov	edi, [esp + 12]	; @to
	mov	ecx, edi
	mov	esi, [esp + 16]	; @from
	
	cld
.m1:	lodsb
	stosb
	test	al, al
	jnz	.m1
	
	mov	eax, ecx
	pop	edi
	pop	esi
	ret

; char *strchr (char *s, int c)
; Look for the first occurence of char @c in string @s
; Return: pointer to @c in string or 0
strchr:	
	push	esi
	
	mov	esi, [esp +  8]
	mov	eax, [esp + 12]
	
	cld
	mov	ah, al
.m3:	lodsb
	cmp	ah, al
	je	.m1
	test	al, al
	jne	.m3
	
	mov	esi, 1
.m1:	dec	esi
	mov	eax, esi
	
	pop	esi
	ret

; char *memcpy (char *to, char *from, int n)
memcpy:
	push	esi
	push	edi
	
	mov	edi, [esp + 12]
	mov	edx, edi
	mov	esi, [esp + 16]
	mov	ecx, [esp + 20]
	
	cld
	rep	movsb
	
	mov	eax, edx

	pop	edi
	pop	esi
	ret

; int memcmp (char *s, char *q, int n)
memcmp:
	push	esi
	push	edi
	
	xor	eax, eax
	mov	esi, [esp + 12]
	mov	edi, [esp + 16]
	mov	ecx, [esp + 20]
	
	cld
	repe	cmpsb
	
	je	.m1
	
	mov	eax, 1
	ja	.m1
	neg	eax

.m1:	pop	edi
	pop	esi
	ret

; char *memset (char *s, int c, int n)
memset:
	push	edi
	
	mov	edi, [esp +  8]
	mov	edx, edi
	mov	eax, [esp + 12]
	mov	ecx, [esp + 16]
	
	cld
	rep	stosb
	
	mov	eax, edx
	pop	edi
	ret
