#include "kernel.h"

#include "core/assert.h"
#include "core/debug.h"
#include "core/string.h"

#include <elf.h>
#include <stdbool.h>

#include "fs.h"
#include "mm.h"

#define PAGE_SIZE 0x1000

#define INT_ADD_OVERFLOW_P(a, b) __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)
#define INT_MUL_OVERFLOW_P(a, b) __builtin_mul_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)

struct elf64_file
{
  char   *data;
  size_t  length;

  Elf64_Ehdr *ehdr;
};

static struct elf64_file as_elf64_file(struct boot_file file)
{
  KASSERT(sizeof(Elf64_Ehdr) <= file.length);
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file.data;
  KASSERT(ehdr->e_ident[EI_MAG0] == ELFMAG0);
  KASSERT(ehdr->e_ident[EI_MAG1] == ELFMAG1);
  KASSERT(ehdr->e_ident[EI_MAG2] == ELFMAG2);
  KASSERT(ehdr->e_ident[EI_MAG3] == ELFMAG3);
  KASSERT(ehdr->e_ident[EI_CLASS] == ELFCLASS64);
  KASSERT(ehdr->e_ident[EI_DATA]  == ELFDATA2LSB);

  KASSERT(!INT_MUL_OVERFLOW_P(ehdr->e_shnum, ehdr->e_shentsize));
  KASSERT(!INT_ADD_OVERFLOW_P(ehdr->e_shoff, ehdr->e_shnum * ehdr->e_shentsize));
  KASSERT(ehdr->e_shoff + ehdr->e_shnum * ehdr->e_shentsize <= file.length);

  struct elf64_file elf64_file;
  elf64_file.data   = file.data;
  elf64_file.length = file.length;
  elf64_file.ehdr   = ehdr;
  return elf64_file;
}

static void elf64_iterate_phdr(struct elf64_file file, void(*iterate)(Elf64_Phdr *))
{
  KASSERT(!INT_MUL_OVERFLOW_P(file.ehdr->e_phnum, file.ehdr->e_phentsize));
  KASSERT(!INT_ADD_OVERFLOW_P(file.ehdr->e_phoff, file.ehdr->e_phnum * file.ehdr->e_phentsize));
  KASSERT(file.ehdr->e_phoff + file.ehdr->e_phnum * file.ehdr->e_phentsize <= file.length);
  for(Elf64_Half i = 0; i < file.ehdr->e_phnum; ++i)
  {
    Elf64_Phdr *phdr = (Elf64_Phdr *)(file.data + file.ehdr->e_phoff + i * file.ehdr->e_phentsize);
    KASSERT(!INT_ADD_OVERFLOW_P(phdr->p_offset, phdr->p_filesz));
    KASSERT(phdr->p_offset + phdr->p_filesz <= file.length);
    iterate(phdr); // Check that phdr is sane
  }
}

Elf64_Addr max_end = 0;
void      *data;
void      *memory;

static void kernel_iterate_phdr1(Elf64_Phdr *phdr)
{
  KASSERT(phdr->p_paddr == phdr->p_vaddr);
  Elf64_Addr  addr = phdr->p_paddr;
  Elf64_Xword sz   = phdr->p_filesz > phdr->p_memsz ? phdr->p_filesz : phdr->p_memsz;
  if(max_end < addr + sz)
    max_end = addr + sz;
}

static void kernel_iterate_phdr2(Elf64_Phdr *phdr)
{
  KASSERT(phdr->p_paddr == phdr->p_vaddr);
  Elf64_Addr addr = phdr->p_paddr;
  memset(memory + addr, 0,                     phdr->p_memsz);
  memcpy(memory + addr, data + phdr->p_offset, phdr->p_filesz);
}

void load_kernel()
{
  struct boot_file *file = boot_fs_lookup("kernel");
  KASSERT(file && "No kernel file loaded");

  struct elf64_file elf64_file = as_elf64_file(*file);
  KASSERT(elf64_file.ehdr->e_type    == ET_EXEC);
  KASSERT(elf64_file.ehdr->e_machine == EM_X86_64);
  KASSERT(elf64_file.ehdr->e_version == EV_CURRENT);

  data = elf64_file.data;
  elf64_iterate_phdr(elf64_file, &kernel_iterate_phdr1);
  memory = boot_mm_alloc_pages((max_end + PAGE_SIZE - 1) / PAGE_SIZE);
  elf64_iterate_phdr(elf64_file, &kernel_iterate_phdr2);
}
