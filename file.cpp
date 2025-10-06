#include "file.h"
#include <assert.h>

FILE *my_fopen(const char *filename, const char *type)
{
    assert(filename);
    assert(type);

    FILE *file = fopen(filename, type);
    if (file == NULL)
    {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return nullptr;
    }
    return file;
}
