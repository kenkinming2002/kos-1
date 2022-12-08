#ifndef CORE_SLOT_H
#define CORE_SLOT_H

struct slot;
struct slot_ops
{
  void(*on_emit)(struct slot *slot);
  void(*on_connect)(struct slot *slot);
  void(*on_disconnect)(struct slot *slot);
};

struct slot
{
  struct slot_ops *ops;
  struct slot *next;
  const char *name;
  void *data;
};

void slot_init(struct slot *slot, struct slot_ops *ops, const char *name, void *data);
void slot_emit(struct slot *slot);
void slot_connect(struct slot *slot, struct slot *next);
void slot_disconnect(struct slot *slot);

#endif // CORE_SLOT_H

