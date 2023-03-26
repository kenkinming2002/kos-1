#ifndef RT_CORE_LOG_H
#define RT_CORE_LOG_H

#include <stdarg.h>

#define LOG_TRACE "\033[90m" "TRACE: " "\033[37m"
#define LOG_INFO  "\033[32m" "INFO: "  "\033[37m"
#define LOG_WARN  "\033[33m" "WARN: "  "\033[37m"
#define LOG_CRIT  "\033[33m" "CRIT:"  "\033[37m"
#define LOG_ERROR "\033[31m" "ERROR: " "\033[37m"

void logc(char c);
void logs(const char *s);
void logf(const char *format, ...);
void vlogf(const char *format, va_list ap);

#endif // RT_CORE_LOG_H
