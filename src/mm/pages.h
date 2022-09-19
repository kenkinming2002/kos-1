#ifndef MM_PAGES_H
#define MM_PAGES_H

void mm_init_pages_allocator();

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // MM_PAGES_H
