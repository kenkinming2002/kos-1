#ifndef X86_PAGING_H
#define X86_PAGING_H

#define PAGE_PRESENT         ((uint64_t)1 << 0)
#define PAGE_WRITABLE        ((uint64_t)1 << 1)
#define PAGE_USER            ((uint64_t)1 << 2)
#define PAGE_WRITE_THROUGH   ((uint64_t)1 << 3)
#define PAGE_UNCACHED        ((uint64_t)1 << 4)
#define PAGE_ACCESSED        ((uint64_t)1 << 5)
#define PAGE_DIRTY           ((uint64_t)1 << 6)
#define PAGE_SIZE_EXTENSION  ((uint64_t)1 << 7)
#define PAGE_GLOBAL          ((uint64_t)1 << 8)
#define PAGE_EXECUTE_DISABLE ((uint64_t)1 << 63)

#define PML1_SHIFT 12
#define PML2_SHIFT 21
#define PML3_SHIFT 30
#define PML4_SHIFT 39

#define PML1_SIZE ((uint64_t)1 << PML1_SHIFT)
#define PML2_SIZE ((uint64_t)1 << PML2_SHIFT)
#define PML3_SIZE ((uint64_t)1 << PML3_SHIFT)
#define PML4_SIZE ((uint64_t)1 << PML4_SHIFT)

#define PML1_MASK ~(PML1_SIZE-1)
#define PML2_MASK ~(PML2_SIZE-1)
#define PML3_MASK ~(PML3_SIZE-1)
#define PML4_MASK ~(PML4_SIZE-1)

typedef uint64_t pme_t;
typedef uint64_t pm_t[512];

#endif // X86_PAGING_H
