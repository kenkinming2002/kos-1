#include "fs.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

struct fs fs;

void fs_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;

      KASSERT(fs.count < MAX_FILES);

      kstrcpy(fs.files[fs.count].name, module_tag->cmdline, sizeof fs.files[fs.count].name);
      fs.files[fs.count].addr   = module_tag->mod_start;
      fs.files[fs.count].length = module_tag->mod_end - module_tag->mod_start;

      ++fs.count;
    }
  }

  debug_printf("fs\n");
  for(size_t i=0; i<fs.count; ++i)
    debug_printf(" => name=%s, addr=0x%lx, length=0x%lx\n",
        fs.files[i].name,
        fs.files[i].addr,
        fs.files[i].length);
}
