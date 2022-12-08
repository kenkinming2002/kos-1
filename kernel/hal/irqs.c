#include "irqs.h"

#include "mm.h"

#include <core/assert.h>
#include <core/string.h>

#include <stddef.h>

struct irqs
{
  struct ll_node node;

  struct module *module;
  uint16_t begin;
  uint16_t count;
};

LL_DEFINE(irqs_list);
int acquire_irqs(struct module *module, unsigned begin, unsigned count)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->begin + irqs->count > begin && begin + count > irqs->begin)
      return -1; // Overlap
  }

  struct irqs *irqs = kmalloc(sizeof *irqs);
  irqs->module  = module;
  irqs->begin   = begin;
  irqs->count   = count;
  ll_append(&irqs_list, &irqs->node);
  return 0;
}

int release_irqs(struct module *module, unsigned begin, unsigned count)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->module == module && irqs->begin == begin && irqs->count == count)
    {
      ll_delete(&irqs->node);
      kfree(irqs);
      return 0;
    }
  }
  return -1;
}


struct slot irq_slots[256];
void isr(uint64_t irq, uint64_t ec)
{
  slot_emit(&irq_slots[irq]);
}

