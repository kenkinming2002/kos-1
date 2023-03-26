#ifndef RT_CORE_FORMAT_H
#define RT_CORE_FORMAT_H

#include <stdarg.h>
#include <stddef.h>

void format(char *buf, size_t n, const char *format, ...);
void vformat(char *buf, size_t n, const char *format, va_list ap);

#endif // RT_CORE_FORMAT_H
