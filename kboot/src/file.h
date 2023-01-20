#ifndef BOOT_FILE_H
#define BOOT_FILE_H

#include <stddef.h>

void fcpy(char *out_data, size_t out_length, size_t out_offset, const char *in_data, size_t in_length, size_t in_offset, size_t n);
void fset(char *data, size_t length, size_t offset, int c, size_t n);
void fread(const char *data, size_t length, size_t offset, char *buf, size_t n);
void fwrite(char *data, size_t length, size_t offset, const char *buf, size_t n);

#endif // BOOT_FILE_H
