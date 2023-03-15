#include "irq.h"

#include <core/assert.h>

#include <stddef.h>
#include <stdint.h>

struct slot root_irq_slots[256];
struct slot exception_irq_slots[32];
struct slot isa_irq_slots[16];
struct slot gsi_irq_slots[208];

struct slot *irq_slot(enum irq_bus_tag tag, unsigned n)
{
  switch(tag)
  {
  case IRQ_BUS_ROOT:      return &root_irq_slots[n];
  case IRQ_BUS_EXCEPTION: return &exception_irq_slots[n];
  case IRQ_BUS_ISA:       return &isa_irq_slots[n];
  case IRQ_BUS_GSI:       return &gsi_irq_slots[n];
  }
  KASSERT_UNREACHABLE;
}

int irq_init()
{
  // 1: Initialize slots
  for(unsigned i=0; i<256; ++i) { slot_init(&root_irq_slots     [i]); root_irq_slots     [i].name = "root"; }
  for(unsigned i=0; i<32; ++i)  { slot_init(&exception_irq_slots[i]); exception_irq_slots[i].name = "exception"; }
  for(unsigned i=0; i<16; ++i)  { slot_init(&isa_irq_slots      [i]); isa_irq_slots      [i].name = "isa"; }
  for(unsigned i=0; i<208; ++i) { slot_init(&gsi_irq_slots      [i]); gsi_irq_slots      [i].name = "gsi"; }

  // 2: Connect exception slots
  for(unsigned i=0; i<32; ++i)
    slot_connect(&root_irq_slots[i], &exception_irq_slots[i]);

  return 0;
}

void isr(uint64_t irq, uint64_t /*ec*/)
{
  if(irq < 256)
    slot_emit(&root_irq_slots[irq]);
}
