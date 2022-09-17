#include "mapping.h"

#include "paging.h"

#define KERNEL_IDENTITY_MAP_BEGIN 0x0
#define KERNEL_IDENTITY_MAP_END   0xC0000000

__attribute__((aligned(4096))) struct pde kernel_pd[1024];

void mm_init_kernel_identity_mapping()
{
  unsigned i = 0;
  for(uint32_t address = KERNEL_IDENTITY_MAP_BEGIN; address < KERNEL_IDENTITY_MAP_END; address += HUGE_PAGE_SIZE)
    kernel_pd[i++] = page_directory_entry_make_huge_page(address, PAGE_CACHED, PAGE_WRITE_BACK, PAGE_ACCESS_SUPERVISOR, PAGE_READ_WRITE);
}

void mm_init_kernel_mapping()
{
  mm_init_kernel_identity_mapping();
  asm volatile(
      "mov %%cr4, %%eax;"
      "or $0x00000010, %%eax;"
      "mov %%eax, %%cr4;"

      "mov %[pd], %%eax;"
      "mov %%eax, %%cr3;"

      "mov %%cr0, %%eax;"
      "or  $0x80000001, %%eax;"
      "mov %%eax, %%cr0;"
      :
      : [pd]"r"(kernel_pd)
      : "memory", "%eax"
  );
}

