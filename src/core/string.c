#include "string.h"

static char *_memmove_forward(char *dst, const char *src, size_t n)
{
  for(size_t i=0; i<n; ++i)
    dst[i] = src[i];
  return dst;
}

static char *_memmove_backward(char *dst, const char *src, size_t n)
{
  for(size_t i=n-1; i<n; --i)
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
