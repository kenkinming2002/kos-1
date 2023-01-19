#include "kernel.h"

#include <kcore/assert.h>
#include <kcore/debug.h>
#include <kcore/string.h>

#include <elf.h>

#include "config.h"
#include "file.h"

__attribute__((section(".kernel"))) static char kernel_area[KERNEL_MAX];

static void ehdr_read(const char *data, size_t length, Elf64_Ehdr *ehdr)
{
  fread(data, length, 0, (char*)ehdr, sizeof *ehdr);
}

static size_t phdr_read_count(const char *data, size_t length)
{
  Elf64_Ehdr ehdr;
  ehdr_read(data, length, &ehdr);
  return ehdr.e_phnum;
}

static void phdr_read(const char *data, size_t length, size_t i, Elf64_Phdr *phdr)
{
  Elf64_Ehdr ehdr;
  ehdr_read(data, length, &ehdr);
  fread(data, length, ehdr.e_phoff + i * ehdr.e_phentsize, (char*)phdr, sizeof *phdr);
}

static void ehdr_check(const Elf64_Ehdr *ehdr)
{
  KASSERT(ehdr->e_ident[EI_MAG0] == ELFMAG0);
  KASSERT(ehdr->e_ident[EI_MAG1] == ELFMAG1);
  KASSERT(ehdr->e_ident[EI_MAG2] == ELFMAG2);
  KASSERT(ehdr->e_ident[EI_MAG3] == ELFMAG3);
  KASSERT(ehdr->e_ident[EI_CLASS] == ELFCLASS64);
  KASSERT(ehdr->e_ident[EI_DATA]  == ELFDATA2LSB);
}

static void kernel_lookup(struct multiboot_boot_information *mbi, char **data, size_t *length)
{
  MULTIBOOT_FOREACH_TAG(mbi, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;
      if(strcmp(module_tag->cmdline, "kernel") == 0)
      {
        *data   = (char*)(uintptr_t)module_tag->mod_start;
        *length = module_tag->mod_end - module_tag->mod_start;
        return;
      }
    }
  }
  KASSERT_UNREACHABLE;
}

void load_kernel(struct multiboot_boot_information *mbi, entry_t *entry)
{
  char  *kernel_data;
  size_t kernel_length;

  Elf64_Ehdr ehdr;
  Elf64_Phdr phdr;

  Elf64_Dyn dyn;

  Elf64_Addr  dt_rela    = 0;
  Elf64_Xword dt_relasz  = 0;
  Elf64_Xword dt_relaent = 0;

  Elf64_Rela rela;

  // 1: Lookup the kernel which is a multiboot2 module
  kernel_lookup(mbi, &kernel_data, &kernel_length);

  // 2: Read and check the elf header
  ehdr_read(kernel_data, kernel_length, &ehdr);
  ehdr_check(&ehdr);

  // 3: Read and process the program header
  size_t phdr_count = phdr_read_count(kernel_data, kernel_length);
  for(size_t i=0; i<phdr_count; ++i)
  {
    phdr_read(kernel_data, kernel_length, i, &phdr);
    switch(phdr.p_type)
    {
    case PT_DYNAMIC:
      for(size_t addr = phdr.p_offset; fread(kernel_data, kernel_length, addr, (char*)&dyn, sizeof dyn), dyn.d_tag != DT_NULL; addr += sizeof dyn)
        switch(dyn.d_tag)
        {
        case DT_RELA:    dt_rela    = dyn.d_un.d_ptr; break;
        case DT_RELASZ:  dt_relasz  = dyn.d_un.d_val; break;
        case DT_RELAENT: dt_relaent = dyn.d_un.d_val; break;
        }
      break;
    case PT_LOAD:
      fset(kernel_area, sizeof kernel_area, phdr.p_vaddr, 0, phdr.p_memsz);
      fcpy(kernel_area, sizeof kernel_area, phdr.p_vaddr, kernel_data, kernel_length, phdr.p_offset, phdr.p_filesz);
      break;
    }
  }

  // 4: Process relocation
  for(size_t offset = dt_rela; offset < dt_rela + dt_relasz; offset += dt_relaent)
  {
    fread(kernel_area, sizeof kernel_area, offset, (char*)&rela, sizeof rela);
    KASSERT(ELF64_R_SYM(rela.r_info)  == 0);
    KASSERT(ELF64_R_TYPE(rela.r_info) == R_X86_64_RELATIVE);

    uint64_t value = (uint64_t)&kernel_area + rela.r_addend;
    fwrite(kernel_area, sizeof kernel_area, rela.r_offset, (char*)&value, sizeof value);
  }

  // 5: Entry point
  *entry = (entry_t)&kernel_area[ehdr.e_entry];
}
