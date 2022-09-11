#include "core/print.h"
#include "debug.h"
#include "multiboot2.h"

#include <stdint.h>
#include <stddef.h>

void kmain(uint32_t magic, uint32_t addr)
{
  debug_init();

  debug_printf("magic = %lx, addr = %lx\n", magic, addr);
  debug_printf("Magic %s\n", magic == MULTIBOOT2_BOOTLOADER_MAGIC ? "match" : "mismatch");

  debug_write("Boot Information Begin\n");
  for(struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uintptr_t)tag + tag->size + 7 & (~7)))
  {
    debug_write("New tag\n");
    switch(tag->type)
    {
    case MULTIBOOT_TAG_TYPE_END:              debug_write("MULTIBOOT_TAG_TYPE_END\n"); break;
    case MULTIBOOT_TAG_TYPE_CMDLINE:          debug_write("MULTIBOOT_TAG_TYPE_CMDLINE\n"); break;
    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: debug_write("MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME\n"); break;
    case MULTIBOOT_TAG_TYPE_MODULE:           debug_write("MULTIBOOT_TAG_TYPE_MODULE\n"); break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:    debug_write("MULTIBOOT_TAG_TYPE_BASIC_MEMINFO\n"); break;
    case MULTIBOOT_TAG_TYPE_BOOTDEV:          debug_write("MULTIBOOT_TAG_TYPE_BOOTDEV\n"); break;
    case MULTIBOOT_TAG_TYPE_MMAP:             debug_write("MULTIBOOT_TAG_TYPE_MMAP\n"); break;
    case MULTIBOOT_TAG_TYPE_VBE:              debug_write("MULTIBOOT_TAG_TYPE_VBE\n"); break;
    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:      debug_write("MULTIBOOT_TAG_TYPE_FRAMEBUFFER\n"); break;
    case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:     debug_write("MULTIBOOT_TAG_TYPE_ELF_SECTIONS\n"); break;
    case MULTIBOOT_TAG_TYPE_APM:              debug_write("MULTIBOOT_TAG_TYPE_APM\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI32:            debug_write("MULTIBOOT_TAG_TYPE_EFI32\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI64:            debug_write("MULTIBOOT_TAG_TYPE_EFI64\n"); break;
    case MULTIBOOT_TAG_TYPE_SMBIOS:           debug_write("MULTIBOOT_TAG_TYPE_SMBIOS\n"); break;
    case MULTIBOOT_TAG_TYPE_ACPI_OLD:         debug_write("MULTIBOOT_TAG_TYPE_ACPI_OLD\n"); break;
    case MULTIBOOT_TAG_TYPE_ACPI_NEW:         debug_write("MULTIBOOT_TAG_TYPE_ACPI_NEW\n"); break;
    case MULTIBOOT_TAG_TYPE_NETWORK:          debug_write("MULTIBOOT_TAG_TYPE_NETWORK\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI_MMAP:         debug_write("MULTIBOOT_TAG_TYPE_EFI_MMAP\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI_BS:           debug_write("MULTIBOOT_TAG_TYPE_EFI_BS\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI32_IH:         debug_write("MULTIBOOT_TAG_TYPE_EFI32_IH\n"); break;
    case MULTIBOOT_TAG_TYPE_EFI64_IH:         debug_write("MULTIBOOT_TAG_TYPE_EFI64_IH\n"); break;
    case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:   debug_write("MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR\n"); break;
    default: debug_write("Unknown Tag\n");
    }
  }
  debug_write("Boot Information End\n");
}
