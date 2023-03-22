#ifndef COER_HAL_MODULE_H
#define COER_HAL_MODULE_H

#include <core/ll.h>
#include <stdbool.h>

/* There are four functions that are provided by any module
 *  - init/fini
 *  - up/down
 *
 * Functions init/fini are used to initialize and finalize module structures.
 * NO device registration MUST be done in init and NO device deregistration
 * MUST be done in fini. Both of them may fail, and they could be retried.
 *
 * Functions up/down are used for registration/deregistration to module
 * structures. Both functions cannot fail, since why would simple
 * registration/deregistration fail?
 *
 * All call to the four functions are synchronized through the use of
 * module_lock spin lock defined in module.c which additionally protect the
 * module list. They MUST not be called directly.
 *
 * TODO: Handle module parameters */
struct module
{
  struct ll_node node;

  const char *name;

  int(*init)();
  int(*fini)();

  void(*up)();
  void(*down)();
};

#define DEFINE_MODULE(_name)                                                                      \
  int _name##_module_init();                                                                      \
  int  _name##_module_fini();                                                                     \
  void _name##_module_up();                                                                       \
  void _name##_module_down();                                                                     \
  struct module _name##_module = {                                                                \
    .name = #_name,                                                                               \
    .init = _name##_module_init,                                                                  \
    .fini = _name##_module_fini,                                                                  \
    .up   = _name##_module_up,                                                                    \
    .down = _name##_module_down,                                                                  \
  };                                                                                              \
  __attribute__((section(".module"), used)) struct module * p_##_name##_module = &_name##_module; \
  static                                    struct module *THIS_MODULE         = &_name##_module;

int module_add(struct module *module);
int module_del(struct module *module);
void module_init();

#endif // CORE_HAL_MODULE_H

