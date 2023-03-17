#ifndef COER_HAL_MODULE_H
#define COER_HAL_MODULE_H

#include <core/ll.h>
#include <stdbool.h>

struct module
{
  struct ll_node node;

  const char *name;
  int(*init)(); // TODO: Module Paramters
  void(*fini)();
};

#define DEFINE_MODULE(_name)                                                                      \
  int _name##_module_init();                                                                      \
  void _name##_module_fini();                                                                      \
  struct module _name##_module = {                                                                \
    .name = #_name,                                                                               \
    .init = _name##_module_init,                                                                  \
    .fini = _name##_module_fini,                                                                  \
  };                                                                                              \
  __attribute__((section(".module"), used)) struct module * p_##_name##_module = &_name##_module; \
  static                                    struct module *THIS_MODULE         = &_name##_module;

int module_add(struct module *module);
void module_del(struct module *module);
void module_init();

#endif // CORE_HAL_MODULE_H

