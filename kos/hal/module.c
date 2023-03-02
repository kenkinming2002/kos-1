#include "module.h"

#include <core/debug.h>

static struct ll module_list = LL_INIT(module_list);

void module_register(struct module *module)
{
  ll_append(&module_list, &module->node);
}

void module_deregister(struct module *module)
{
  ll_delete(&module->node);
}

void module_update()
{
  LL_FOREACH(module_list, node)
  {
    struct module *module = (struct module *)node;
    if(!module->initialized)
    {
      // TODO: LOG_* macro for log level
      debug_printf("\033[91m" "INFO" "\033[37m" ": initializing module %s\n", module->name);
      module->initialized = true;
      module->init();
    }
  }
}

