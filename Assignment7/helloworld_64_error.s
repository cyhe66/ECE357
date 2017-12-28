.text

.global _start

_start:
        mov $9000,		%rax
        mov $1,			%rdi
        mov $message,	%rsi
		mov $13,		%rdx
        SYSCALL

		mov $60,		%rax
		mov $5,			%rdi
	
message:
        .ascii "Hello World\n"



