#include "module.h"

#include <core/log.h>
#include <arch/locks.h>

struct spin_lock module_lock;
struct ll        module_list = LL_INIT(module_list);

int module_add(struct module *module)
{
  int error = 0;

  logf(LOG_INFO "adding module - name = {:s}\n", module->name);

  spin_lock(&module_lock);
  ll_append(&module_list, &module->node);
  error = module->init();
  if(error != 0)
    goto out;

  module->up();

out:
  spin_unlock(&module_lock);
  return error;
}

int module_del(struct module *module)
{
  logf(LOG_INFO "deleting module - name = {:s}\n", module->name);

  int error = 0;

  spin_lock(&module_lock);
  module->down();
  error = module->fini();
  if(error != 0)
    goto out;

  ll_delete(&module->node);

out:
  spin_unlock(&module_lock);
  return error;
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

