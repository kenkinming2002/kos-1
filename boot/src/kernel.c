#include "kernel.h"

#include <boot/service.h>

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

Elf64_Addr  dt_rela;
Elf64_Xword dt_relasz;    /* total size */
Elf64_Xword dt_relaent;   /* entry size */

static void kernel_iterate_phdr1(Elf64_Phdr *phdr)
{
  if(phdr->p_type != PT_LOAD)
    return;

  KASSERT(phdr->p_paddr == phdr->p_vaddr);
  Elf64_Addr  addr = phdr->p_paddr;
  Elf64_Xword sz   = phdr->p_filesz > phdr->p_memsz ? phdr->p_filesz : phdr->p_memsz;
  if(max_end < addr + sz)
    max_end = addr + sz;
}

static void kernel_iterate_phdr2(Elf64_Phdr *phdr)
{
  if(phdr->p_type != PT_LOAD)
    return;

  KASSERT(phdr->p_paddr == phdr->p_vaddr);
  Elf64_Addr addr = phdr->p_paddr;
  memset(memory + addr, 0,                     phdr->p_memsz);
  memcpy(memory + addr, data + phdr->p_offset, phdr->p_filesz);
}

static void kernel_iterate_phdr3(Elf64_Phdr *phdr)
{
  if(phdr->p_type != PT_DYNAMIC)
    return;

  KASSERT(phdr->p_paddr == phdr->p_vaddr);
  Elf64_Addr addr = phdr->p_paddr;

  Elf64_Dyn *dyns = (Elf64_Dyn *)(memory + addr);
  for(Elf64_Dyn *dyn = dyns; dyn->d_tag != DT_NULL; ++dyn)
    switch(dyn->d_tag)
    {
    case DT_RELA:      dt_rela      = dyn->d_un.d_ptr; break;
    case DT_RELASZ:    dt_relasz    = dyn->d_un.d_val; break;
    case DT_RELAENT:   dt_relaent   = dyn->d_un.d_val; break;
    }
}

extern struct boot_service boot_service;
typedef void(*entry_t)(struct boot_service *);

void load_kernel()
{
  struct boot_file *file = boot_fs_lookup("kernel");
  KASSERT(file && "No kernel file loaded");

  struct elf64_file elf64_file = as_elf64_file(*file);
  KASSERT(elf64_file.ehdr->e_type    == ET_DYN);
  KASSERT(elf64_file.ehdr->e_machine == EM_X86_64);
  KASSERT(elf64_file.ehdr->e_version == EV_CURRENT);

  data = elf64_file.data;
  elf64_iterate_phdr(elf64_file, &kernel_iterate_phdr1);
  memory = boot_mm_alloc(max_end, PAGE_SIZE);
  elf64_iterate_phdr(elf64_file, &kernel_iterate_phdr2);
  elf64_iterate_phdr(elf64_file, &kernel_iterate_phdr3);

  // Perform relocations
  KASSERT(!INT_ADD_OVERFLOW_P(dt_rela, dt_relasz));
  for(uintptr_t addr = dt_rela; addr < dt_rela + dt_relasz; addr += dt_relaent)
  {
    Elf64_Rela *rela = (Elf64_Rela *)(memory + addr);

    KASSERT(ELF64_R_SYM(rela->r_info)  == 0);
    KASSERT(ELF64_R_TYPE(rela->r_info) == R_X86_64_RELATIVE);
    KASSERT(rela->r_offset <= max_end);
    *(uint64_t *)(memory + rela->r_offset) = (uint64_t)(memory + rela->r_addend);

    KASSERT(!INT_ADD_OVERFLOW_P(addr, dt_relaent));
  }

  debug_printf("entry = 0x%lx\n", elf64_file.ehdr->e_entry);
  debug_printf("max_end = 0x%lx\n", max_end);
  KASSERT(elf64_file.ehdr->e_entry <= max_end);
  entry_t entry = (entry_t)(memory + elf64_file.ehdr->e_entry);
  entry(&boot_service);
  KASSERT(false && "Unreachable");
}
