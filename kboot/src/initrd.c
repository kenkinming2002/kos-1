#include "initrd.h"

#include "config.h"
#include "file.h"

#include <core/assert.h>
#include <core/string.h>

__attribute__((section(".initrd"))) static char initrd_area[INITRD_MAX];

static void initrd_lookup(struct multiboot_boot_information *mbi, char **data, size_t *length)
{
  MULTIBOOT_FOREACH_TAG(mbi, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;
      if(strcmp(module_tag->cmdline, "initrd") == 0)
      {
        *data   = (char*)(uintptr_t)module_tag->mod_start;
        *length = module_tag->mod_end - module_tag->mod_start;
        return;
      }
    }
  }
  KASSERT_UNREACHABLE;
}

void load_initrd(struct multiboot_boot_information *mbi, const char **data, size_t *length)
{
  char  *initrd_data;
  size_t initrd_length;

  initrd_lookup(mbi, &initrd_data, &initrd_length);
  fcpy(initrd_area, sizeof initrd_area, 0, initrd_data, initrd_length, 0, initrd_length);

  *data   = initrd_data;
  *length = initrd_length;
}
