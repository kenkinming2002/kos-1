#include "fs.h"

#include "boot_params.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

#define BOOT_FS boot_params.fs

void fs_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;

      KASSERT(BOOT_FS.count < MAX_FILES);

      kstrcpy(BOOT_FS.files[BOOT_FS.count].name, module_tag->cmdline, sizeof BOOT_FS.files[BOOT_FS.count].name);
      BOOT_FS.files[BOOT_FS.count].addr   = module_tag->mod_start;
      BOOT_FS.files[BOOT_FS.count].length = module_tag->mod_end - module_tag->mod_start;

      ++BOOT_FS.count;
    }
  }

  debug_printf("fs\n");
  for(size_t i=0; i<BOOT_FS.count; ++i)
    debug_printf(" => name=%s, addr=0x%lx, length=0x%lx\n",
        BOOT_FS.files[i].name,
        BOOT_FS.files[i].addr,
        BOOT_FS.files[i].length);
}
