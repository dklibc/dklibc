; Fasm source
; Swap bytes to network byte order (x86_32 little endian to big endian)

format ELF

public htons
public htonl
public ntohs
public ntohl

section ".text" executable

; unsigned short htons (unsigned short c)
; unsigned short ntohs (unsigned short c)
ntohs:
htons:
	mov	eax, [esp + 4]
	xchg	ah, al
	ret

; unsigned htonl (unsigned c)
; unsigned ntohl (unsigned c)
ntohl:
htonl:
	mov	eax, [esp + 4]
	bswap	eax
	ret
