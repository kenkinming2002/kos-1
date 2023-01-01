#ifndef MODULE_H
#define MODULE_H

struct module
{
  const char *name;
};

#define DEFINE_MODULE(_name) static struct module this_module = { .name = #_name }
#define THIS_MODULE (&this_module)

#endif // MODULE_H

