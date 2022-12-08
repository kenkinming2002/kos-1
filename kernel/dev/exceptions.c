#include "exceptions.h"

#include "hal/irqs.h"
#include "mm.h"

#include <core/assert.h>

DEFINE_MODULE(exceptions);

struct exceptions
{
};

static void exceptions_init(struct exceptions *exceptions)
{
}

static void exceptions_fini(struct exceptions *exceptions)
{
}

static struct exceptions exceptions;
void exceptions_module_init()
{
  exceptions_init(&exceptions);
}

