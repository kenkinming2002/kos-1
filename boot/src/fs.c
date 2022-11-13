#include "fs.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

#define MAX_FILES           32
#define MAX_FILENAME_LENGTH 256

struct boot_file
{
  char name[MAX_FILENAME_LENGTH];

  char  *data;
  size_t length;
};

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

      kstrcpy(files[count].name, module_tag->cmdline, sizeof files[count].name);
      files[count].data   = (char *)(uintptr_t)module_tag->mod_start;
      files[count].length = module_tag->mod_end - module_tag->mod_start;

      ++count;
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

void boot_fs_read(struct boot_file *file, size_t offset, size_t length, char *buffer)
{
  KASSERT(offset <= offset + length);
  KASSERT(offset + length <= file->length);
  memcpy(buffer, &file->data[offset], length);
}

void boot_fs_write(struct boot_file *file, size_t offset, size_t length, const char *buffer)
{
  KASSERT(offset <= offset + length);
  KASSERT(offset + length <= file->length);
  memcpy(&file->data[offset], buffer, length);
}

