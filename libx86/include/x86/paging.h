#ifndef X86_PAGING_H
#define X86_PAGING_H

#define PAGE_PRESENT         ((uint64_t)1 << 0)
#define PAGE_WRITABLE        ((uint64_t)1 << 1)
#define PAGE_USER            ((uint64_t)1 << 2)
#define PAGE_WRITE_THROUGH   ((uint64_t)1 << 3)
#define PAGE_UNCACHED        ((uint64_t)1 << 4)
#define PAGE_ACCESSED        ((uint64_t)1 << 5)
#define PAGE_DIRTY           ((uint64_t)1 << 6)
#define PAGE_SIZE            ((uint64_t)1 << 7)
#define PAGE_GLOBAL          ((uint64_t)1 << 8)
#define PAGE_EXECUTE_DISABLE ((uint64_t)1 << 63)

typedef uint64_t pme_t;
typedef uint64_t pm_t[512];

#endif // X86_PAGING_H
