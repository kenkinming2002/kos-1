#include "module.h"

#include <core/log.h>

static struct ll module_list = LL_INIT(module_list);

void module_register(struct module *module)
{
  ll_append(&module_list, &module->node);
}

void module_deregister(struct module *module)
{
  ll_delete(&module->node);
}

void module_init()
{
  extern struct module *module_begin[];
  extern struct module *module_end[];

  for(struct module **module = module_begin; module != module_end; ++module)
    if(*module)
      module_register(*module);

  module_update();
}

void module_update()
{
  LL_FOREACH(module_list, node)
  {
    struct module *module = (struct module *)node;
    if(!module->initialized)
    {
      // TODO: LOG_* macro for log level
      logf("\033[91m" "INFO" "\033[37m" ": initializing module {:s}\n", module->name);
      module->initialized = true;
      module->init();
    }
  }
}

