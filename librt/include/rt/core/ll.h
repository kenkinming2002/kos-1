#ifndef RT_CORE_LL_H
#define RT_CORE_LL_H

#include <stdbool.h>

struct ll_node
{
  struct ll_node *prev, *next;
};

void ll_insert_before(struct ll_node *at, struct ll_node *node);
void ll_insert_after(struct ll_node *at, struct ll_node *node);
void ll_delete(struct ll_node *node);

struct ll
{
  struct ll_node sentinel;
};

#define LL_INIT(name) (struct ll){ .sentinel = { .prev = &name.sentinel, .next = &name.sentinel } }
#define LL_FOREACH(name, node) for(struct ll_node *node = name.sentinel.next; node != &name.sentinel; node = node->next)

bool ll_empty(struct ll *ll);
void ll_prepend(struct ll *ll, struct ll_node *node);
void ll_append(struct ll *ll, struct ll_node *node);

#endif // RT_CORE_LL_H
