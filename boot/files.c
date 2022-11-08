#include "files.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

struct file files[MAX_FILES];
size_t      file_count;

void files_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;

      KASSERT(file_count < MAX_FILES);

      kstrcpy(files[file_count].name, module_tag->cmdline, sizeof files[file_count].name);
      files[file_count].addr   = module_tag->mod_start;
      files[file_count].length = module_tag->mod_end - module_tag->mod_start;

      ++file_count;
    }
  }

  debug_printf("files\n");
  for(size_t i=0; i<file_count; ++i)
    debug_printf(" => name=%s, addr=0x%lx, length=0x%lx\n",
        files[i].name,
        files[i].addr,
        files[i].length);
}
