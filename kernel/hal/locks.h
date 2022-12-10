#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdatomic.h>

struct local_lock
{
  uint64_t flags;
};

void local_lock(struct local_lock *lock);
void local_unlock(struct local_lock *lock);

#define LOCAL_LOCK_INIT (struct local_lock){0}

struct spin_lock
{
  uint64_t flags;
  uint64_t value;
};

void spin_lock(struct spin_lock *lock);
void spin_unlock(struct spin_lock *lock);

#define SPIN_LOCK_INIT (struct spin_lock){0, 0}

#endif // LOCK_SPINLOCK_H
