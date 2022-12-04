#include "exceptions.h"

#include "hal/irqs.h"
#include "mm.h"

#include <core/assert.h>

#include <stdbool.h>
#include <stddef.h>

struct cpu_exceptions
{
  struct irqs_source source;
};

static void cpu_exceptions_set_base(struct irqs_source *source, unsigned base)
{
  // There is no way to relocate interrupt vectors for cpu exceptions
  KASSERT(base == 0);
}

static struct cpu_exceptions *cpu_exceptions_create()
{
  // NOTE: There is no need to acknowledge, unmask or mask cpu exceptions
  struct cpu_exceptions *cpu_exceptions = kmalloc(sizeof *cpu_exceptions);
  cpu_exceptions->source.set_base    = &cpu_exceptions_set_base;
  cpu_exceptions->source.acknowledge = NULL;
  cpu_exceptions->source.unmask      = NULL;
  cpu_exceptions->source.mask        = NULL;
  return cpu_exceptions;
}

static void cpu_exceptions_destroy(struct cpu_exceptions *cpu_exceptions)
{
  kfree(cpu_exceptions);
}

struct cpu_exceptions *cpu_exceptions;
void exceptions_init()
{
  cpu_exceptions = cpu_exceptions_create();
  // TODO: Attach it
}

