#ifndef CORE_PRINT_H
#define CORE_PRINT_H

void debug_put(char c);
void debug_write(const char *str);
__attribute__((format(printf, 1, 2))) void debug_printf(const char *format, ...);

#endif // CORE_PRINT_H
