#include "file.h"

#include <rt/core/assert.h>
#include <rt/core/string.h>

#define INT_ADD_OVERFLOW_P(a, b) __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)

void fcpy(char *out_data, size_t out_length, size_t out_offset, const char *in_data, size_t in_length, size_t in_offset, size_t n)
{
  KASSERT(!INT_ADD_OVERFLOW_P(out_offset, n));
  KASSERT(!INT_ADD_OVERFLOW_P(in_offset,  n));
  KASSERT(out_offset + n <= out_length);
  KASSERT(in_offset  + n <= in_length);
  memcpy(&out_data[out_offset], &in_data[in_offset], n);
}

void fset(char *data, size_t length, size_t offset, int c, size_t n)
{
  KASSERT(!INT_ADD_OVERFLOW_P(offset, n));
  KASSERT(offset + n <= length);
  memset(&data[offset], c, n);
}

void fread(const char *data, size_t length, size_t offset, char *buf, size_t n)
{
  KASSERT(!INT_ADD_OVERFLOW_P(offset, n));
  KASSERT(offset + n <= length);
  memcpy(buf, &data[offset], n);
}

void fwrite(char *data, size_t length, size_t offset, const char *buf, size_t n)
{
  KASSERT(!INT_ADD_OVERFLOW_P(offset, n));
  KASSERT(offset + n <= length);
  memcpy(&data[offset], buf, n);
}

