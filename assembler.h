#ifndef ASSEMBLER_H
#define ASSEMBLER_H

size_t GetFileSize(const char *file_name);
int *TranslateToByteCode(FILE *f, int *code_size);
int WriteByteCode(FILE *f, int *code, int size);
int GetRegNumber(const char *reg_name);

#endif //ASSEMBLER_H
