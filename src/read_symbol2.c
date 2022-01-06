#include "read_symbol.h"

void read_symbol_table(Filedata * filedata) {

    rewind(filedata->file);

    fseek(filedata->file, filedata->section_headers[filedata->file_header.e_shstrndx].sh_offset, SEEK_SET);
    // 第e_shstrndx项?~X??~W符串表 ?~Z?~I ?~W?~J~B ?~U?度 char类?~^~K ?~U??~D
    char shstrtab[filedata->section_headers[filedata->file_header.e_shstrndx].sh_size];
    char *temp = shstrtab;

    // 读?~O~V?~F~E容
    size_t a = fread(shstrtab, filedata->section_headers[filedata->file_header.e_shstrndx].sh_size, 1, filedata->file);
    if (0 == a){
        printf("\nfail to read\n");
    }
    int index_strtab = 0;
    for (int i=0; i < filedata->file_header.e_shnum; i++){
        temp=shstrtab;
        temp=temp+filedata->section_headers[i].sh_name;
        if(strcmp(temp, ".strtab")!=0) continue;
        index_strtab=i;
    }
    printf("%d\n", index_strtab);
    rewind(filedata->file);
    fseek(filedata->file, filedata->section_headers[index_strtab].sh_offset, SEEK_SET);
    char strtab[filedata->section_headers[index_strtab].sh_size];
    a = fread(strtab, filedata->section_headers[index_strtab].sh_size, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
    }
    char *strtemp=strtab;
    // ?~A~M?~N~F

    for (int i = 0; i < filedata->file_header.e_shnum; i++){
        temp = shstrtab;
        temp = temp + filedata->section_headers[i].sh_name;
        if (strcmp(temp, ".symtab") != 0) continue;
        int number_sym = filedata->section_headers[i].sh_size / filedata->section_headers[i].sh_entsize;
        printf("La table de symboles << .symtab >> contient %d entrees :\n", number_sym);
        Elf32_Sym *symtable=(Elf32_Sym*)malloc(sizeof(Elf32_Sym) * number_sym);
        if (symtable == NULL){
            fprintf(stderr, "Not able to allocate memory for symtable %i", i);
        }


        fseek(filedata->file, filedata->section_headers[i].sh_offset, SEEK_SET); //?~I??~H?表?~Z~D?~M置
        printf("Size:%d\n",filedata->section_headers[i].sh_size);
        
        if (fread(symtable, sizeof(Elf32_Sym)*number_sym, 1, filedata->file)){
            fprintf(stderr, "Unable to read file\n");
        }

        printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
        for (int j=0; j<number_sym; j++) {
            temp=shstrtab;
            printf("     %d ", j);
            printf("%08x\t", symtable[j].st_value);
            printf("%d\t", symtable[j].st_size);
            switch (ELF32_ST_TYPE(symtable[j].st_info)) {
                case 0:
                    printf("NOTYPE\t");
                    break;
                case 1:
                    printf("OBJECT\t");
                    break;
                case 2:
                    printf("FUNC\t");
                    break;
                case 3:
                    printf("SECTION\t");
                    break;
                case 4:
                    printf("FILE\t");
                    break;
                case 13:
                    printf("LOPROC\t");
                    break;
                case 15:
                    printf("HIPROC\t");
                    break;

                default:break;
            }
            switch(ELF32_ST_BIND(symtable[j].st_info))
            {
                case 0: printf("LOCAL\t\t");
                    break;
                case 1: printf("GLOBAL\t\t");
                    break;
                case 2: printf("WEAK\t\t");
                    break;
                case 3: printf("NUM\t\t");
                    break;
                case 13: printf("LOPROC\t\t");
                    break;
                case 15: printf("HIPROC\t\t");
                    break;

                default:
                    break;
            }
            switch(symtable[j].st_other){
                case 0: printf("DEFAULT\t\t");
                    break;
                case 2: printf("HIDDEN\t\t");
                    break;
                default:
                    printf("%hu\t", symtable[j].st_other);
                    break;
            }
            switch(symtable[j].st_shndx){
                case 0: printf("UND\t");
                    break;
                case 0xfff1: printf("ABS\t\t");
                    break;
                case 0xfff2: printf("COMMON\t\t");
                    break;
                default: printf("%hu\t", symtable[j].st_shndx);
                    break;

            }
            strtemp=strtab;
            strtemp=strtemp+symtable[j].st_name;
            printf("NAME: %s\n", strtemp);
        }

    }
    return;
}