#ifndef RT_CORE_ONCE_H
#define RT_CORE_ONCE_H

#define ONCE_SYNC 0x1

struct once
{
  unsigned value;
  unsigned done;
};

int once_begin(struct once *once, unsigned flag);
void once_end(struct once *once, unsigned flag);

#endif // RT_CORE_ONCE_H
