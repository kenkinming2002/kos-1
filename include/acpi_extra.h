#ifndef ACPI_EXTRA_H
#define ACPI_EXTRA_H

#include "acpi.h"
#include "multiboot2_extra.h"

#include <kcore/string.h>

#include <stddef.h>

static struct RSDP *mb2_get_rsdp(struct multiboot_boot_information *mbi)
{
  MULTIBOOT_FOREACH_TAG(mbi, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_ACPI_NEW)
    {
      struct multiboot_tag_new_acpi *acpi_tag = (struct multiboot_tag_new_acpi *)tag;
      return (struct RSDP *)&acpi_tag->rsdp;
    }

  MULTIBOOT_FOREACH_TAG(mbi, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_ACPI_OLD)
    {
      struct multiboot_tag_old_acpi *acpi_tag = (struct multiboot_tag_old_acpi *)tag;
      return (struct RSDP *)&acpi_tag->rsdp;
    }

  return NULL;
}

static struct RSDT *rsdp_get_rsdt(struct RSDP *rsdp) { return (struct RSDT *)(uintptr_t)rsdp->rsdt_address; }
static struct XSDT *rsdp_get_xsdt(struct RSDP *rsdp) { return (struct XSDT *)(uintptr_t)rsdp->xsdt_address; }

static size_t rsdt_count(struct RSDT *rsdt) { return (rsdt->sdt.length - sizeof *rsdt) / sizeof rsdt->ptrs[0]; }
static size_t xsdt_count(struct XSDT *xsdt) { return (xsdt->sdt.length - sizeof *xsdt) / sizeof xsdt->ptrs[0]; }

static struct SDT *rsdt_find_sdt(struct RSDT *rsdt, char signature[4])
{
  for(size_t i=0; i<rsdt_count(rsdt); ++i)
  {
    struct SDT *sdt = (struct SDT *)(uintptr_t)rsdt->ptrs[i];
    if(memcmp(sdt->signature, signature, sizeof sdt->signature) == 0)
      return sdt;
  }
  return NULL;
}

static struct SDT *xsdt_find_sdt(struct XSDT *xsdt, char signature[4])
{
  for(size_t i=0; i<xsdt_count(xsdt); ++i)
  {
    struct SDT *sdt = (struct SDT *)(uintptr_t)xsdt->ptrs[i];
    if(memcmp(sdt->signature, signature, sizeof sdt->signature) == 0)
      return sdt;
  }
  return NULL;
}

static struct MADTEntry *madt_begin(struct MADT *madt)
{
  return (struct MADTEntry *)((uintptr_t)madt + sizeof *madt);
}

static struct MADTEntry *madt_end(struct MADT *madt)
{
  return (struct MADTEntry *)((uintptr_t)madt + madt->sdt.length);
}

static struct MADTEntry *madt_next(struct MADTEntry *entry)
{
  return (struct MADTEntry *)((uintptr_t)entry + entry->length);
}

#endif // ACPI_EXTRA_H
