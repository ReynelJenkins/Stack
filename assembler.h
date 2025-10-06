#ifndef ASSEMBLER_H
#define ASSEMBLER_H

size_t GetFileSize(const char *file_name);
FILE *my_fopen(const char *filename, const char *type);
int *TranslateToByteCode(FILE *f, int *code_size);
int WriteByteCode(FILE *f, int *code, int size);
int GetRegNumber(const char *reg_name);

#endif //ASSEMBLER_H
