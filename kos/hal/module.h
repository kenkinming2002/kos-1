#ifndef MODULE_H
#define MODULE_H

#include <core/ll.h>
#include <stdbool.h>

struct module
{
  struct ll_node node;

  const char *name;
  int(*init)(); // TODO: Module Paramters
  int(*fini)();

  bool initialized;
};

#define DEFINE_MODULE(_name)                                           \
  int _name##_module_init();                                           \
  int _name##_module_fini();                                           \
  __attribute__((section(".module"))) struct module _name##_module = { \
    .name = #_name,                                                    \
    .init = _name##_module_init,                                       \
    .fini = _name##_module_fini,                                       \
    .initialized = false,                                              \
  };                                                                   \
  static struct module *THIS_MODULE = &_name##_module;

void module_register(struct module *module);
void module_deregister(struct module *module);

void module_update();

#endif // MODULE_H

