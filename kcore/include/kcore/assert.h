#ifndef CORE_ASSERT_H
#define CORE_ASSERT_H

#include <stdbool.h>

__attribute__((noreturn)) void kassert(const char *file, int line);
#define KASSERT(expr) do { if(!(expr)) kassert(__FILE__, __LINE__); } while(0)
#define KASSERT_UNREACHABLE KASSERT(false && "Unreachable")

#endif // CORE_ASSERT_H
