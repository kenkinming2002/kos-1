#ifndef CORE_LL_H
#define CORE_LL_H

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

void ll_prepend(struct ll *ll, struct ll_node *node);
void ll_append(struct ll *ll, struct ll_node *node);

#endif // CORE_LL_H
