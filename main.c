#include <stdio.h>
#include <stdint.h>
#include <elf.h>

// segment must be page-aligned as the linux kernel can only give out
// pages with a size of 4096 bytes, and as .text and .data have different
// permissions, alignment is required
#define SGM_TEXT_SIZE 4096
#define SGM_DATA_SIZE 4096

int main(void) {

    int virt_addr = 0x400000;
    int elf_hdr_size = 64;
    int prog_hdr_size = 56;
    int text_offset = elf_hdr_size + prog_hdr_size*2;
    int data_offset = text_offset + SGM_TEXT_SIZE;

    Elf64_Ehdr elf_hdr = {
        .e_ident = {
            [EI_MAG0]       = ELFMAG0,             // magic number
            [EI_MAG1]       = ELFMAG1,             // magic number
            [EI_MAG2]       = ELFMAG2,             // magic number
            [EI_MAG3]       = ELFMAG3,             // magic number
            [EI_CLASS]      = ELFCLASS64,          // 64-bit
            [EI_DATA]       = ELFDATA2LSB,         // little endian
            [EI_VERSION]    = EV_CURRENT,          // version
            [EI_OSABI]      = 0x0,                 // sysv abi
            [EI_ABIVERSION] = 0x0,                 // ignored
            [EI_PAD]        = 0x0,                 // padding
        },
        .e_type      = ET_EXEC,                    // executable
        .e_machine   = EM_X86_64,                  // amd64 isa
        .e_version   = EV_CURRENT,                 // elf version
        .e_entry     = virt_addr + text_offset,    // entry point
        .e_phoff     = elf_hdr_size,               // start of program header table
        .e_shoff     = 0,                          // start of section header table
        .e_flags     = 0x0,                        // ignored
        .e_ehsize    = elf_hdr_size,               // size of this header
        .e_phentsize = prog_hdr_size,              // size of program header table entry
        .e_phnum     = 2,                          // # entries in program header table
        .e_shentsize = 0x40,                       // size of section header table entry
        .e_shnum     = 0,                          // # entries in section header table
        .e_shstrndx  = SHN_UNDEF,                  // index of section header table that contains section names
    };

    Elf64_Phdr prog_hdr_text = {
        .p_type   = PT_LOAD,
        .p_flags  = PF_X + PF_R,
        .p_offset = text_offset,
        .p_vaddr  = virt_addr + text_offset,
        .p_paddr  = 0, // ignored
        .p_filesz = SGM_TEXT_SIZE,
        .p_memsz  = SGM_TEXT_SIZE,
        .p_align  = 4096,
    };

    Elf64_Phdr prog_hdr_data = {
        .p_type   = PT_LOAD,
        .p_flags  = PF_R + PF_W,
        .p_offset = data_offset,
        .p_vaddr  = virt_addr + data_offset,
        .p_paddr  = 0, // ignored
        .p_filesz = SGM_DATA_SIZE,
        .p_memsz  = SGM_DATA_SIZE,
        .p_align  = 4096,
    };

    uint8_t sgm_text[SGM_TEXT_SIZE] = {
        0xb8, 0x3c, 0x0, 0x0, 0x0, // mov rax, 60
        0xbf, 0x2d, 0x0, 0x0, 0x0, // mov rdi, 45
        0x0F, 0x05, // syscall
    };

    uint8_t sgm_data[SGM_DATA_SIZE] = {
        45
    };

    FILE *fp = fopen("elf", "wb");
    fwrite(&elf_hdr, sizeof(uint8_t), sizeof(elf_hdr), fp);
    fwrite(&prog_hdr_text, sizeof(uint8_t), sizeof(prog_hdr_text), fp);
    fwrite(&prog_hdr_data, sizeof(uint8_t), sizeof(prog_hdr_data), fp);
    fwrite(&sgm_text, sizeof(uint8_t), sizeof(sgm_text), fp);
    fwrite(&sgm_data, sizeof(uint8_t), sizeof(sgm_data), fp);
    fclose(fp);

    return 0;
}
