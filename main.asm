section .text
global _start
_start:
push rbp
mov rbp, rsp

mov rax, 60
mov rdi, 45
syscall

mov rsp, rbp
pop rbp
