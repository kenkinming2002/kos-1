#include "irqs.h"

#include "mm.h"

#include <core/ll.h>
#include <stddef.h>

#define MAX_IRQ 256

struct irq_domain
{
  struct ll_node node;

  unsigned base;
  unsigned count;

  void(*handler)(unsigned, void*);
  void *data;
};
LL_DEFINE(irq_domains);

struct irq_domain *irq_alloc_domain(unsigned base, unsigned count, void(*handler)(unsigned, void*), void *data)
{
  if(base == IRQ_DOMAIN_DYNAMIC_BASE)
  {
    if(count > MAX_IRQ)
      return NULL;

    for(unsigned base=0; base<MAX_IRQ-count; ++base)
    {
      struct irq_domain *domain = irq_alloc_domain(base, count, handler, data);
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
  domain->handler = handler;
  domain->data    = data;
  ll_append(&irq_domains, &domain->node);
  return domain;
}

void irq_free_domain(struct irq_domain *domain)
{
  ll_delete(&domain->node);
  kfree(domain);
}

void irq_handle(unsigned irq)
{
  LL_FOREACH(irq_domains, node)
  {
    struct irq_domain *domain = (struct irq_domain *)node;
    if(domain->base <= irq && irq < domain->base + domain->count)
      return domain->handler(irq - domain->base, domain->data);
  }
}
