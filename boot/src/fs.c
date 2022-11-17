#include "fs.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

#define MAX_FILES     32
#define MAX_FILENAMES 256

static const char *filename_allocate(const char *filename)
{
  static char   storage[MAX_FILENAMES] = {};
  static size_t index                  = 0;

  KASSERT(index + strlen(filename) + 1 <= MAX_FILENAMES);
  char *new_filename = &storage[index];
  index += strlen(filename) + 1;

  strcpy(new_filename, filename);
  return new_filename;
}

struct boot_file files[MAX_FILES];
size_t           count;

void fs_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;

      KASSERT(count != MAX_FILES);
      files[count++] = (struct boot_file){
        .name   = filename_allocate(module_tag->cmdline),
        .data   = (char *)(uintptr_t)module_tag->mod_start,
        .length = module_tag->mod_end - module_tag->mod_start,
      };
    }
  }

  debug_printf("fs\n");
  for(size_t i=0; i<count; ++i)
    debug_printf(" => name=%s, addr=0x%lx, length=0x%lx\n",
        files[i].name,
        (uintptr_t)files[i].data,
        files[i].length);
}

void boot_fs_iterate(void(*iterate)(struct boot_file *file))
{
  for(size_t i=0; i<count; ++i)
    iterate(&files[i]);
}

