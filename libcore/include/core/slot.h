#ifndef CORE_SLOT_H
#define CORE_SLOT_H

struct slot;
struct slot_ops
{
  void(*on_emit)(struct slot *slot);

  void(*on_connect_prev)(struct slot *slot);
  void(*on_disconnect_prev)(struct slot *slot);

  void(*on_connect_next)(struct slot *slot);
  void(*on_disconnect_next)(struct slot *slot);
};

struct slot
{
  struct slot_ops *ops;
  struct slot *prev, *next;
  const char *name;
  void *data;
};

void slot_init(struct slot *slot, struct slot_ops *ops, const char *name, void *data);

void slot_emit_backward(struct slot *slot);
void slot_emit_forward(struct slot *slot);

void slot_connect(struct slot *prev, struct slot *next);
void slot_disconnect(struct slot *prev, struct slot *next);

#endif // CORE_SLOT_H

