#include "debug.h"
#include "multiboot2.h"

#include <stdint.h>
#include <stddef.h>

void kmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_write("ABCDEFG ");

  if(magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    debug_write("Magic mismatch");
  else
    debug_write("Magic match");

  debug_write("Boot Information Begin ");
  for(struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uintptr_t)tag + tag->size + 7 & (~7)))
  {
    debug_write("New tag ");
    switch(tag->type)
    {
    case MULTIBOOT_TAG_TYPE_END:              debug_write("MULTIBOOT_TAG_TYPE_END"); break;
    case MULTIBOOT_TAG_TYPE_CMDLINE:          debug_write("MULTIBOOT_TAG_TYPE_CMDLINE"); break;
    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: debug_write("MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME"); break;
    case MULTIBOOT_TAG_TYPE_MODULE:           debug_write("MULTIBOOT_TAG_TYPE_MODULE"); break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:    debug_write("MULTIBOOT_TAG_TYPE_BASIC_MEMINFO"); break;
    case MULTIBOOT_TAG_TYPE_BOOTDEV:          debug_write("MULTIBOOT_TAG_TYPE_BOOTDEV"); break;
    case MULTIBOOT_TAG_TYPE_MMAP:             debug_write("MULTIBOOT_TAG_TYPE_MMAP"); break;
    case MULTIBOOT_TAG_TYPE_VBE:              debug_write("MULTIBOOT_TAG_TYPE_VBE"); break;
    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:      debug_write("MULTIBOOT_TAG_TYPE_FRAMEBUFFER"); break;
    case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:     debug_write("MULTIBOOT_TAG_TYPE_ELF_SECTIONS"); break;
    case MULTIBOOT_TAG_TYPE_APM:              debug_write("MULTIBOOT_TAG_TYPE_APM"); break;
    case MULTIBOOT_TAG_TYPE_EFI32:            debug_write("MULTIBOOT_TAG_TYPE_EFI32"); break;
    case MULTIBOOT_TAG_TYPE_EFI64:            debug_write("MULTIBOOT_TAG_TYPE_EFI64"); break;
    case MULTIBOOT_TAG_TYPE_SMBIOS:           debug_write("MULTIBOOT_TAG_TYPE_SMBIOS"); break;
    case MULTIBOOT_TAG_TYPE_ACPI_OLD:         debug_write("MULTIBOOT_TAG_TYPE_ACPI_OLD"); break;
    case MULTIBOOT_TAG_TYPE_ACPI_NEW:         debug_write("MULTIBOOT_TAG_TYPE_ACPI_NEW"); break;
    case MULTIBOOT_TAG_TYPE_NETWORK:          debug_write("MULTIBOOT_TAG_TYPE_NETWORK"); break;
    case MULTIBOOT_TAG_TYPE_EFI_MMAP:         debug_write("MULTIBOOT_TAG_TYPE_EFI_MMAP"); break;
    case MULTIBOOT_TAG_TYPE_EFI_BS:           debug_write("MULTIBOOT_TAG_TYPE_EFI_BS"); break;
    case MULTIBOOT_TAG_TYPE_EFI32_IH:         debug_write("MULTIBOOT_TAG_TYPE_EFI32_IH"); break;
    case MULTIBOOT_TAG_TYPE_EFI64_IH:         debug_write("MULTIBOOT_TAG_TYPE_EFI64_IH"); break;
    case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:   debug_write("MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR"); break;
    default: debug_write("Unknown Tag");
    }
    debug_write(" ");
  }
  debug_write("Boot Information End ");
}
