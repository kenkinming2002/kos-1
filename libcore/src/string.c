#include "core/string.h"

#include "core/debug.h"

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

size_t strlen(const char *s)
{
  size_t i=0;
  while(*s++) ++i;
  return i;
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

