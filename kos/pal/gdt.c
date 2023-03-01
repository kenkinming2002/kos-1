#include "gdt.h"

#include "segs.h"

#include <stdint.h>

struct __attribute__((packed)) gdt_entry
{
  union
  {
    uint64_t value;

    struct __attribute__((packed))
    {
      uint16_t limit_low;
      uint16_t base_low;

      uint8_t base_high1;
      uint8_t type    : 4;
      uint8_t dtype   : 1; /* descriptor type(0=system, 1=code or data) */
      uint8_t dpl     : 2;
      uint8_t present : 1;

      uint8_t limit_high : 4;
      uint8_t avail : 1; /* available for use */
      uint8_t lm    : 1; /* 64-bit code segment */
      uint8_t sz    : 1; /* default operation size(0=16-bit, 1=32-bit) */
      uint8_t gran  : 1; /* granularity */
      uint8_t base_high2;
    };

    struct __attribute__((packed))
    {
      uint32_t base;
      uint32_t reserved;
    };
  };
};

struct __attribute__((packed)) gdt_descriptor
{
  uint16_t limit;
  uint64_t base;
};

struct __attribute__((packed)) tss
{
  uint32_t reserved1;

  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;

  uint64_t reserved2;

  uint64_t ist0;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;

  uint64_t reserved3;

  uint16_t reserved4;
  uint16_t io_map_base;
};

/* 0:   null descriptor
 * 1:   64-bit kernel code segment descriptor
 * 2:   64-bit kernel data segment descriptor
 * 3:   64-bit user   code segment descriptor
 * 4:   64-bit user   data segment descriptor
 * 5-6: tss descriptor(2 entries) */
struct gdt_entry gdt[7];
struct gdt_descriptor gdt_desc = {
  .limit = sizeof gdt - 1,
  .base  = (uint64_t)&gdt,
};

struct tss tss;

void gdt_init()
{
  gdt[0].value = 0;

  gdt[1] = (struct gdt_entry){
    .base_low   = 0,
    .base_high1 = 0,
    .base_high2 = 0,

    .limit_low  = 0xFFFF,
    .limit_high = 0xF,

    .type      = 0xA,
    .dtype     = 1,
    .dpl       = 0,
    .present   = 1,

    .avail        = 0,
    .lm           = 1,
    .sz           = 0,
    .gran         = 1,
  };

  gdt[2] = (struct gdt_entry){
    .base_low   = 0,
    .base_high1 = 0,
    .base_high2 = 0,

    .limit_low  = 0xFFFF,
    .limit_high = 0xF,

    .type      = 0x2,
    .dtype     = 1,
    .dpl       = 0,
    .present   = 1,

    .avail        = 0,
    .lm           = 0,
    .sz           = 1,
    .gran         = 1,
  };

  gdt[3] = (struct gdt_entry){
    .base_low   = 0,
    .base_high1 = 0,
    .base_high2 = 0,

    .limit_low  = 0xFFFF,
    .limit_high = 0xF,

    .type      = 0xA,
    .dtype     = 1,
    .dpl       = 3,
    .present   = 1,

    .avail        = 0,
    .lm           = 1,
    .sz           = 0,
    .gran         = 1,
  };

  gdt[4] = (struct gdt_entry){
    .base_low   = 0,
    .base_high1 = 0,
    .base_high2 = 0,

    .limit_low  = 0xFFFF,
    .limit_high = 0xF,

    .type      = 0x2,
    .dtype     = 1,
    .dpl       = 3,
    .present   = 1,

    .avail        = 0,
    .lm           = 0,
    .sz           = 1,
    .gran         = 1,
  };

  gdt[5] = (struct gdt_entry){
    .base_low   = ((uint64_t)&tss >> 0)  & 0xFFFF,
    .base_high1 = ((uint64_t)&tss >> 16) & 0xFF,
    .base_high2 = ((uint64_t)&tss >> 24) & 0xFF,

    .limit_low  = (sizeof tss >> 0 ) & 0xFFFF,
    .limit_high = (sizeof tss >> 16) & 0xF,

    .type      = 0xA,
    .dtype     = 0,
    .dpl       = 0,
    .present   = 1,

    .avail        = 0,
    .lm           = 0,
    .sz           = 0,
    .gran         = 0,
  };

  gdt[6] = (struct gdt_entry){
    .base     = ((uint64_t)&tss >> 32) & 0xFFFF,
    .reserved = 0,
  };
}

void gdt_load()
{
  asm volatile ("lgdt %0" : : "m"(gdt_desc));
  reload_segs();
}

