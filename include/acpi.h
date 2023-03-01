#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

#define ACPI_PACKED __attribute__((packed))

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

struct RSDT
{
  struct SDT sdt;
  uint32_t ptrs[];
};

struct XSDT
{
  struct SDT sdt;
  uint64_t ptrs[];
};

struct MADT
{
  struct SDT sdt;
  uint32_t lapic_address;
  uint32_t flags;
};

#define PCAT_COMPACT 0x1

struct ACPI_PACKED MADTEntry
{
  uint8_t type;
  uint8_t length;
  union {
    struct ACPI_PACKED {
      uint8_t  processor_uid;
      uint8_t  id;
      uint32_t flags;
    } lapic;

    struct ACPI_PACKED {
      uint8_t  ioapic_id;
      uint8_t  reserved;
      uint32_t address;
      uint32_t gsi_base;
    } ioapic;

    struct ACPI_PACKED {
      uint8_t  bus;
      uint8_t  source;
      uint32_t gsi;
      uint16_t flags;
    } interrupt_source_override;

    struct ACPI_PACKED {
      uint16_t flags;
      uint32_t gsi;
    } nmi_source;

    struct ACPI_PACKED {
      uint8_t  processor_uid;
      uint16_t flags;
      uint8_t  lint;
    } lapic_nmi;

    struct ACPI_PACKED {
      uint16_t reserved;
      uint64_t address;
    } lapic_address_override;

    struct ACPI_PACKED {
      uint16_t reserved;
      uint32_t id;
      uint32_t flags;
      uint32_t processor_uid;
    } lx2apic;

    struct ACPI_PACKED {
      uint16_t flags;
      uint32_t processor_uid;
      uint8_t  lint;
      uint8_t  reserved[3];
    } lx2apic_nmi;
  };
};

#define MADT_LAPIC                      0x0
#define MADT_IOAPIC                     0x1
#define MADT_INTERRUPT_SOURCE_OVERRIDE  0x2
#define MADT_NMI_SOURCE                 0x3
#define MADT_LAPIC_NMI                  0x4
#define MADT_LAPIC_ADDRESS_OVERRIDE     0x5
#define MADT_LX2APIC                    0x9
#define MADT_LX2APIC_NMI                0xA

#define LAPIC_ENABLED        0x1
#define LAPIC_ONLINE_CAPABLE 0x2

#define MPS_INTI_POLARITY_MASK (0x3 << 0)
#define MPS_INTI_ACTIVE_HIGH   (0x1 << 0)
#define MPS_INTI_ACTIVE_LOW    (0x3 << 0)

#define MPS_INTI_TRIGGER_MODE_MASK (0x3 << 2)
#define MPS_INTI_EDGE_TRIGGERED    (0x1 << 2)
#define MPS_INTI_LEVEL_TRIGGERED   (0x3 << 2)

#endif // ACPI_H
