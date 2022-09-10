#ifndef TYPES_ASSERT_H
#define TYPES_ASSERT_H

void kassert(const char *file, int line);
#define KASSERT(expr) do { if(!(expr)) kassert(__FILE__, __LINE__); } while(0)

#endif // TYPES_ASSERT_H
