#ifndef CORE_LOG_H
#define CORE_LOG_H

#include <stdarg.h>

void logc(char c);
void logs(const char *s);
void logf(const char *format, ...);
void vlogf(const char *format, va_list ap);

#endif // CORE_LOG_H
