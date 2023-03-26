#include <rt/core/string.h>

static char *_memcpy(char *restrict dst, const char *restrict src, size_t n)
{
  for(size_t i=0; i<n; ++i)
    dst[i] = src[i];
  return dst;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t n)
{
  return _memcpy(dst, src, n);
}

static char *_memmove_forward(char *dst, const char *src, size_t n)
{
  for(size_t i=n-1; i<n; --i)
    dst[i] = src[i];
  return dst;
}

static char *_memmove_backward(char *dst, const char *src, size_t n)
{
  for(size_t i=0; i<n; ++i)
    dst[i] = src[i];
  return dst;
}

static char *_memmove(char *dst, const char *src, size_t n)
{
  // Check if it does not overlap
  if(src+n<=dst || dst+n<=src)
    return _memmove_forward(dst, src, n);

  if(src<dst)
    return _memmove_forward(dst, src, n);

  if(dst<src)
    return _memmove_backward(dst, src, n);

  return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
  return _memmove(dst, src, n);
}

static char *_memset(char *dst, int c, size_t n)
{
  for(size_t i=0; i<n; ++i)
    dst[i] = c;
  return dst;
}

void *memset(void *dst, int c, size_t n)
{
  return _memset(dst, c, n);
}

int _memcmp(const char *s1, const char *s2, size_t n)
{
  for(size_t i=0; i<n; ++i)
    if(s1[i] != s2[i])
      return s1[i] - s2[i];
  return 0;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
  return _memcmp(s1, s2, n);
}

char *strcpy(char *restrict dst, const char *restrict src)
{
  char *res = dst;
  while((*dst++ = *src++) != '\0');
  return res;
}

char *strncpy(char *restrict dst, const char *restrict src, size_t n)
{
  char *res = dst;
  while(n-- && (*dst++ = *src++) != '\0');
  return res;
}

size_t strlen(const char *s)
{
  size_t i=0;
  while(*s++) ++i;
  return i;
}

size_t strnlen(const char *s, size_t n)
{
  size_t i=0;
  while(n-- && *s++) ++i;
  return i;
}

static int _strcmp(const unsigned char *s1, const unsigned char *s2)
{
  while(*s1 && *s2 && *s1 == *s2) ++s1, ++s2;
  return (int)*s1 - (int)*s2;
}

int strcmp(const char *s1, const char *s2)
{
  return _strcmp((const unsigned char *)s1, (const unsigned char *)s2);
}

static int _strncmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
  while(n-- && *s1 && *s2 && *s1 == *s2) ++s1, ++s2;
  if(n == (size_t)-1)
    return 0;

  return (int)*s1 - (int)*s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  return _strncmp((const unsigned char *)s1, (const unsigned char *)s2, n);
}

size_t kstrcpy(char *restrict dst, const char *restrict src, size_t size)
{
  size_t len = strlen(src);
  if(len >= size - 1)
    len = size - 1;

  memcpy(dst, src, len);
  dst[len] = '\0';
  return len;
}

