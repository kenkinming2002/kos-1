#include "irqs.h"

#include "mm.h"

#include <core/assert.h>
#include <core/string.h>

#include <stddef.h>

struct irq_line
{
  struct module *module;

  handler_t  handler;
  void      *data;
};

struct irqs
{
  struct ll_node node;

  struct module *module;
  uint16_t begin;
  uint16_t count;

  struct irq_line    *lines;
  struct irqs_source *source;
};

LL_DEFINE(irqs_list);
int acquire_irqs(struct module *module, unsigned begin, unsigned count, struct irqs_source *source)
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

  irqs->lines   = kmalloc(sizeof *irqs->lines * irqs->count);
  memset(irqs->lines, 0, sizeof *irqs->lines * irqs->count);

  irqs->source = source;
  irqs->source->set_base(irqs->source, irqs->begin);

  ll_append(&irqs_list, &irqs->node);
  return 0;
}

int release_irqs(struct module *module, unsigned begin, unsigned count, struct irqs_source *source)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->module == module && irqs->begin == begin && irqs->count == count && irqs->source == source)
    {
      kfree(irqs->lines);
      ll_delete(&irqs->node);
      kfree(irqs);
      return 0;
    }
  }
  return -1;
}

int irqs_register_handler(struct irqs_source *source, struct module *module, unsigned irq, handler_t handler, void *data)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->source == source)
    {
      KASSERT(irq < irqs->count);
      if(irqs->lines[irq].module != NULL)
        return -1;

      irqs->lines[irq].module  = module;
      irqs->lines[irq].handler = handler;
      irqs->lines[irq].data    = data;

      if(irqs->source->unmask)
        irqs->source->unmask(irqs->source, irq);

      return 0;
    }
  }
  return -1;
}

int irqs_deregister_handler(struct irqs_source *source, struct module *module, unsigned irq)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->source == source)
    {
      KASSERT(irq < irqs->count);
      if(irqs->lines[irq].module != module)
        return -1;

      irqs->lines[irq].module  = NULL;
      irqs->lines[irq].handler = NULL;
      irqs->lines[irq].data    = NULL;

      if(irqs->source->mask)
        irqs->source->mask(irqs->source, irq);

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

