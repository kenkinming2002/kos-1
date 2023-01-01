#include <kcore/ll.h>

void ll_insert_before(struct ll_node *at, struct ll_node *node)
{
  node->prev = at->prev;
  node->next = at;
  node->prev->next = node;
  node->next->prev = node;
}

void ll_insert_after(struct ll_node *at, struct ll_node *node)
{
  node->prev = at;
  node->next = at->next;
  node->prev->next = node;
  node->next->prev = node;
}

void ll_delete(struct ll_node *node)
{
  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->prev = node->next = node;
}

void ll_prepend(struct ll *ll, struct ll_node *node)
{
  ll_insert_after(&ll->sentinel, node);
}

void ll_append(struct ll *ll, struct ll_node *node)
{
  ll_insert_before(&ll->sentinel, node);
}
