#include "i8259.h"

#include "hal/irqs.h"
#include "mm.h"

#include <core/assert.h>
#include <core/slot.h>

DEFINE_MODULE(i8259);

#define PIC_MASTER 0x0020
#define PIC_SLAVE  0x00A0

#define ICW1_ICW4	0x01
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	0x08
#define ICW1_INIT	0x10

#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

struct i8259
{
  struct i8259 *master;
  uint16_t ports;
  uint8_t  base;
  uint8_t  config;
  uint8_t  mask;

  struct slot slots[8];
};

static void i8259_slot_on_connect(struct slot *slot)
{
  struct i8259 *pic = slot->data;
  unsigned      irq = pic->slots - slot;

  // Unmask the interrupt
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask &= ~(1 << irq);
  outb(data_port, mask);
}

static void i8259_slot_on_disconnect(struct slot *slot)
{
  struct i8259 *pic = slot->data;
  unsigned      irq = pic->slots - slot;

  // Mask the interrupt
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask |= 1 << irq;
  outb(data_port, mask);
}

static void i8259_slot_on_emit(struct slot *slot)
{
  struct i8259 *pic = slot->data;

  // Acknowledge the interrupt
  outb(pic->ports, 0x20);
  if(pic->master)
    outb(pic->master->ports, 0x20);
}

struct slot_ops i8259_slot_ops = {
  .on_connect_prev    = NULL,
  .on_disconnect_prev = NULL,
  .on_connect_next    = &i8259_slot_on_connect,
  .on_disconnect_next = &i8259_slot_on_disconnect,
  .on_emit            = &i8259_slot_on_emit,
};

static int i8259_init(struct i8259 *pic, struct i8259 *master, uint16_t ports, uint8_t base, uint8_t config, uint8_t mask)
{
  pic->master = master;
  pic->base   = base;
  pic->ports  = ports;
  pic->config = config;
  pic->mask   = mask;

  if(acquire_irqs(THIS_MODULE, pic->base, 8) != 0)
    return -1;

  if(acquire_ports(THIS_MODULE, pic->ports, 2) != 0)
    return -1;

  uint16_t command_port = pic->ports;
  uint16_t data_port    = pic->ports + 1;
  outb(command_port, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(data_port,    pic->base);             io_wait();
  outb(data_port,    pic->config);           io_wait();
  outb(data_port,    ICW4_8086);             io_wait();
  outb(data_port,    pic->mask);             io_wait();

  for(unsigned i=0; i<8; ++i)
  {
    slot_init(&pic->slots[i], &i8259_slot_ops, "i8259", pic);
    irq_bus_set_output("root", pic->base + i, &pic->slots[i]);
  }
  return 0;
}

static void i8259_fini(struct i8259 *pic)
{
  for(unsigned i=0; i<8; ++i)
    irq_bus_unset_output("root", pic->base + i);

  KASSERT(release_irqs(THIS_MODULE, pic->base, 8) == 0);
  KASSERT(release_ports(THIS_MODULE, pic->ports, 2) == 0);
}

static struct i8259 i8259_master;
static struct i8259 i8259_slave;

void i8259_module_init()
{
  i8259_init(&i8259_master, NULL,          0x20, PIC_MASTER, 1 << 2, 0xFB);
  i8259_init(&i8259_slave,  &i8259_master, 0x28, PIC_SLAVE,  2,      0xFF);

  for(unsigned i=0; i<8; ++i)
  {
    irq_bus_set_input("isa", i,   &i8259_master.slots[i]);
    irq_bus_set_input("isa", i+8, &i8259_slave.slots[i]);
  }
}

