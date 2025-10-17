#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define MAX_LABELS 100

typedef struct {
    char name[32];
    int addr;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;
int SecondRun = 0;

size_t GetFileSize(const char *file_name);
int *TranslateToByteCode(FILE *f, int *code_size);
int WriteByteCode(FILE *f, int *code, int size);
int GetRegNumber(const char *reg_name);
int GetLabelPos(const char *label);

#endif //ASSEMBLER_H
