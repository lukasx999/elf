
a.out:     file format elf64-x86-64


Disassembly of section .text:

0000000000401000 <_start>:
  401000:	55                   	push   %rbp
  401001:	48 89 e5             	mov    %rsp,%rbp
  401004:	b8 3c 00 00 00       	mov    $0x3c,%eax
  401009:	48 8b 3c 25 00 20 40 	mov    0x402000,%rdi
  401010:	00 
  401011:	0f 05                	syscall
  401013:	48 89 ec             	mov    %rbp,%rsp
  401016:	5d                   	pop    %rbp
