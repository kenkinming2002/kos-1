#include "once.h"

int once_begin(struct once *once, unsigned flag)
{
  if(!__atomic_exchange_n(&once->value, 1, __ATOMIC_SEQ_CST))
    return 1;

  if(flag & ONCE_SYNC)
    while(!__atomic_load_n(&once->done, __ATOMIC_ACQUIRE))
      asm volatile("pause");

  return 0;
}

void once_end(struct once *once, unsigned flag)
{
  if(flag & ONCE_SYNC)
    __atomic_store_n(&once->done, 1, __ATOMIC_RELEASE);
}
