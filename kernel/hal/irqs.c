#include "irqs.h"

#include "mm.h"

#include <stddef.h>

#define MAX_IRQ 256

struct irqs
{
  struct ll_node node;

  struct module *module;
  uint16_t begin;
  uint16_t count;

  struct irqs_source *source;
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
  irqs->module = module;
  irqs->begin  = begin;
  irqs->count  = count;
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

int irqs_attach_source(struct module *module, unsigned begin, unsigned count, struct irqs_source *source)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->module == module && irqs->begin == begin && irqs->count == count)
    {
      if(irqs->source != NULL)
        return -1;

      irqs->source = source;
      return 0;
    }
  }
  return -1;
}

int irqs_detach_source(struct module *module, unsigned begin, unsigned count, struct irqs_source *source)
{
  LL_FOREACH(irqs_list, node)
  {
    struct irqs *irqs = (struct irqs *)node;
    if(irqs->module == module && irqs->begin == begin && irqs->count == count)
    {
      if(irqs->source != source)
        return -1;

      irqs->source = NULL;
      return 0;
    }
  }
  return -1;
}

LL_DEFINE(irq_domains);

struct irq_domain *irq_alloc_domain(unsigned base, unsigned count)
{
  if(base == IRQ_DOMAIN_DYNAMIC_BASE)
  {
    if(count > MAX_IRQ)
      return NULL;

    for(unsigned base=0; base<MAX_IRQ-count; ++base)
    {
      struct irq_domain *domain = irq_alloc_domain(base, count);
      if(domain)
        return domain;
    }
    return NULL;
  }

  LL_FOREACH(irq_domains, node)
  {
    struct irq_domain *domain = (struct irq_domain *)node;
    if(base < domain->base + domain->count && domain->base < base + count)
      return NULL; // Overlap
  }
  struct irq_domain *domain = kmalloc(sizeof *domain);
  domain->base    = base;
  domain->count   = count;
  domain->handler = NULL;
  domain->data    = NULL;
  ll_append(&irq_domains, &domain->node);
  return domain;
}

void irq_free_domain(struct irq_domain *domain)
{
  ll_delete(&domain->node);
  kfree(domain);
}

void isr(uint64_t vector, uint64_t ec)
{
  LL_FOREACH(irq_domains, node)
  {
    struct irq_domain *domain = (struct irq_domain *)node;
    if(domain->base <= vector && vector < domain->base + domain->count)
      if(domain->handler)
        return domain->handler(vector - domain->base, domain->data);
  }
}
