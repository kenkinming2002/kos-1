#ifndef CORE_PRINT_H
#define CORE_PRINT_H

__attribute__((format(printf, 1, 2))) void debug_printf(const char *format, ...);

#endif // CORE_PRINT_H
