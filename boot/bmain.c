#include "boot_params.h"

#include "core/debug.h"

#include "debug.h"
#include "mm.h"
#include "modules.h"

void bmain(uint64_t magic, struct multiboot_boot_information *boot_info)
{
  debug_init();
  debug_printf("hello magic=0x%lx, addr=0x%lx\n", magic, (uintptr_t)boot_info);

  boot_params_init(magic, (uintptr_t)boot_info);

  mm_init(boot_info);
  modules_init(boot_info);

  debug_printf("mmap\n");
  for(size_t i=0; i<mmap_entry_count; ++i)
    debug_printf(" => addr=0x%lx, length=0x%lx, type=%u\n",
        mmap_entries[i].addr,
        mmap_entries[i].length,
        mmap_entries[i].type);

  debug_printf("modules\n");
  for(size_t i=0; i<module_count; ++i)
    debug_printf(" => addr=0x%lx, length=0x%lx\n",
        modules[i].addr,
        modules[i].length);

  for(size_t i=0; i<100; ++i)
    debug_printf("mmap alloc = 0x%lx\n", (uintptr_t)mm_alloc(2));

  debug_printf("mmap\n");
  for(size_t i=0; i<mmap_entry_count; ++i)
    debug_printf(" => addr=0x%lx, length=0x%lx, type=%u\n",
        mmap_entries[i].addr,
        mmap_entries[i].length,
        mmap_entries[i].type);
}
