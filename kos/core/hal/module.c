#include "module.h"

#include <core/log.h>

static struct ll module_list = LL_INIT(module_list);

int module_add(struct module *module)
{
  logf(LOG_INFO "adding module - name = {:s}\n", module->name);

  int status;
  if((status = module->init()) != 0)
    return status;

  ll_append(&module_list, &module->node);
  return 0;
}

void module_del(struct module *module)
{
  logf(LOG_INFO "deleting module - name = {:s}\n", module->name);

  ll_delete(&module->node);
  module->fini();
}

void module_init()
{
  extern struct module *module_begin[];
  extern struct module *module_end[];

  for(struct module **module = module_begin; module != module_end; ++module)
    if(*module)
      module_add(*module);

  for(struct module **module = module_begin; module != module_end; ++module)
    if(*module)
      module_del(*module);

  for(struct module **module = module_begin; module != module_end; ++module)
    if(*module)
      module_add(*module);
}

