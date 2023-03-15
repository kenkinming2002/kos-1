#include <core/log.h>

#include <core/format.h>

#define LOG_MAX 256

extern void arch_logc(char c);

void logc(char c)
{
  arch_logc(c);
}

void logs(const char *s)
{
  for(;*s; ++s)
    logc(*s);
}

void logf(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  vlogf(format, ap);
  va_end(ap);
}

void vlogf(const char *format, va_list ap)
{
  char buf[LOG_MAX];
  vformat(buf, sizeof buf, format, ap);
  logs(buf);
}
