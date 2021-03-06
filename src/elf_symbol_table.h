#include "elf_header.h"
#include "elf_main.h"
#include "elf_shdrs.h"
#include <string.h>

#ifndef _ELF_SYMBOL_TABLE_
#define _ELF_SYMBOL_TABLE_

bool get_symbol_table(Filedata * filedata);

bool process_symbol_table(Filedata * filedata);

char * get_st_type(Elf32_Sym *symtable, int i);

char * get_st_bind(Elf32_Sym *symtable, int i);

int get_index_strtab(Filedata * filedata, char * temp, int index_strtab);

int get_index_symtab(Filedata * filedata, char * temp, int index_symtab);

int get_st_value(Elf32_Sym *symtable, int i);

int get_st_size(Elf32_Sym *symtable, int i);

char * get_st_vis(unsigned int other);

char * get_st_name(Elf32_Sym *symtable, int i,char * strtemp);

#endif