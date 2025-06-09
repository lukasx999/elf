section .data
foo:
dq 45

section .text
global _start
_start:
push rbp
mov rbp, rsp

mov rax, 60
mov rdi, [foo]
syscall

mov rsp, rbp
pop rbp
