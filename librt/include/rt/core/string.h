#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <stddef.h>

void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

char *strcpy(char *restrict dst, const char *restrict src);
char *strncpy(char *restrict dst, const char *restrict src, size_t n);

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

/* Copy string from src to a buffer pointed to by dst of length size.
 * Return the length of string copied. */
size_t kstrcpy(char *restrict dst, const char *restrict src, size_t size);

#endif // CORE_STRING_H
