#include "boot_params.h"
#include "multiboot2_extra.h"

#include "core/assert.h"
#include "core/string.h"

#include "mm/linear.h"
#include "multiboot2.h"

struct boot_params boot_params;

static void boot_params_init_multiboot2_cmdline(struct multiboot_tag_string *tag)
{
  kstrcpy(boot_params.cmdline, tag->string, sizeof boot_params.cmdline);
}

static void boot_params_init_multiboot2(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    switch(tag->type)
    {
    case MULTIBOOT_TAG_TYPE_CMDLINE:
      boot_params_init_multiboot2_cmdline((struct multiboot_tag_string *)tag);
      break;
    }
}

void boot_params_init(uint32_t magic, uint32_t addr)
{
  KASSERT(magic == MULTIBOOT2_BOOTLOADER_MAGIC);
  boot_params_init_multiboot2((struct multiboot_boot_information *)addr);
}

