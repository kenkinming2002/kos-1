#ifndef ARCH_ONCE_H
#define ARCH_ONCE_H

#define ONCE_SYNC 0x1

struct once
{
  unsigned value;
  unsigned done;
};

int once_begin(struct once *once, unsigned flag);
void once_end(struct once *once, unsigned flag);

#endif // ARCH_ONCE_H
