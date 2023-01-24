#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

struct RSDP
{
  // Version 1.0
  char     signature[8];
  uint8_t  checksum;
  char     oem_id[6];
  uint8_t  revision;
  uint32_t rsdt_address;

  // Version 2.0
  uint32_t length;
  uint64_t xsdt_address;
  uint8_t  extended_checksum;
  char     reserved[3];
};

struct SDT
{
  char     signature[4];
  uint32_t length;
  uint8_t  revision;
  uint8_t  checksum;
  char     oem_id[6];
  char     oem_table_id[8];
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
};

struct MADT
{
  struct SDT sdt;
  uint32_t lapic_address;
  uint32_t flags;
};

#define PCAT_COMPACT 0x1

struct MADT_Entry
{
  uint8_t type;
  uint8_t length;
};

#define LAPIC                      0x0
#define IOAPIC                     0x1
#define INTERRUPT_SOURCE_OVERRIDE  0x2
#define NMI_SOURCE                 0x3
#define LAPIC_NMI                  0x4
#define LAPIC_ADDRESS_OVERRIDE     0x5
#define LX2APIC                    0x9
#define LX2APIC_NMI                0xA

/* Local xAPIC and Local x2APIC description */
struct LAPICEntry
{
  struct MADT_Entry entry;
  uint8_t  processor_uid;
  uint8_t  id;
  uint32_t flags;
};

struct IOAPICEntry
{
  struct MADT_Entry entry;
  uint8_t  ioapic_id;
  uint8_t  reserved;
  uint32_t ioapic_address;
  uint32_t gsi_base;
};

struct InterruptSourceOverrideEntry
{
  struct MADT_Entry entry;
  uint8_t  bus;
  uint8_t  source;
  uint32_t gsi;
  uint16_t flags;
};

struct NMISourceEntry
{
  struct MADT_Entry entry;
  uint16_t flags;
  uint32_t gsi;
};

struct LAPICNMIEntry
{
  struct MADT_Entry entry;
  uint8_t  processor_uid;
  uint16_t flags;
  uint8_t  lint;
};

struct LAPICAddressOverrideEntry
{
  struct MADT_Entry entry;
  uint16_t reserved;
  uint64_t lapic_address;
};

struct LX2APICEntry
{
  struct MADT_Entry entry;
  uint16_t reserved;
  uint32_t id;
  uint32_t flags;
  uint32_t processor_uid;
};

/* Local APIC and Local x2 APIC NMI description */
struct LX2APICNMIEntry
{
  struct MADT_Entry entry;
  uint16_t flags;
  uint32_t processor_uid;
  uint8_t  lint;
  uint8_t  reserved[3];
};

#define LAPIC_ENABLED        0x1
#define LAPIC_ONLINE_CAPABLE 0x2

#define MPS_INTI_POLARITY_MASK (0x3 << 0)
#define MPS_INTI_ACTIVE_HIGH   (0x1 << 0)
#define MPS_INTI_ACTIVE_LOW    (0x3 << 0)

#define MPS_INTI_TRIGGER_MODE_MASK (0x3 << 2)
#define MPS_INTI_EDGE_TRIGGERED    (0x1 << 2)
#define MPS_INTI_LEVEL_TRIGGERED   (0x3 << 2)

#endif // ACPI_H
