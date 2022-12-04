#include "irqs.h"

#include "mm.h"

#include <stddef.h>

#define MAX_IRQ 256

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

void irq_handle(unsigned irq)
{
  LL_FOREACH(irq_domains, node)
  {
    struct irq_domain *domain = (struct irq_domain *)node;
    if(domain->base <= irq && irq < domain->base + domain->count)
      if(domain->handler)
        return domain->handler(irq - domain->base, domain->data);
  }
}
