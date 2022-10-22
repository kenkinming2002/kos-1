#include "modules.h"

#include "core/assert.h"

struct module modules[MAX_MODULES];
size_t        module_count;

void modules_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;

      KASSERT(module_count < MAX_MODULES);
      modules[module_count++] = (struct module){
        .addr = module_tag->mod_start,
        .length = module_tag->mod_end - module_tag->mod_start,
      };
    }
  }
}
