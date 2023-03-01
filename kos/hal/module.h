#ifndef MODULE_H
#define MODULE_H

struct module
{
  const char *name;

  // TODO: Module Paramters
  int(*init)();
  int(*fini)();
};

#define DEFINE_MODULE(_name)       \
  int _name##_module_init();       \
  int _name##_module_fini();       \
  struct module _name##_module = { \
    .name = #_name,                \
    .init = _name##_module_init,   \
    .fini = _name##_module_fini,   \
  };                               \
  static struct module *THIS_MODULE = &_name##_module;

#endif // MODULE_H

