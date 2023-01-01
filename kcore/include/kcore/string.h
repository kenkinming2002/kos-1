#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);

char *strcpy(char *restrict dst, const char *restrict src);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);

/* Copy string from src to a buffer pointed to by dst of length size.
 * Return the length of string copied. */
size_t kstrcpy(char *restrict dst, const char *restrict src, size_t size);

#endif // STRING_H
