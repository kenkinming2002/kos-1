#ifndef RT_CORE_ASSERT_H
#define RT_CORE_ASSERT_H

__attribute__((noreturn)) void kassert(const char *file, int line);
#define KASSERT(expr) do { if(!(expr)) kassert(__FILE__, __LINE__); } while(0)
#define KASSERT_UNREACHABLE KASSERT(0 && "Unreachable")

#endif // RT_CORE_ASSERT_H
