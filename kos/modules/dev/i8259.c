#include "core/hal/irq.h"
#include "core/hal/res.h"
#include "core/hal/module.h"
#include "core/hal/device.h"

#include <arch/access.h>
#include <core/assert.h>

DEFINE_MODULE(i8259)

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
  struct device device;

  struct i8259 *master;
  uint16_t ports;
  uint8_t  base_root;
  uint8_t  base_isa;
  uint8_t  config;
  uint8_t  mask;

  struct slot slots[8];
};

static void i8259_slot_on_enable(struct slot *slot)
{
  struct i8259 *pic = slot->data;
  unsigned      irq = pic->slots - slot;

  // Unmask the interrupt
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask &= ~(1 << irq);
  outb(data_port, mask);
}

static void i8259_slot_on_disable(struct slot *slot)
{
  struct i8259 *pic = slot->data;
  unsigned      irq = pic->slots - slot;

  // Mask the interrupt
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask |= 1 << irq;
  outb(data_port, mask);
}

static bool i8259_slot_on_emit(struct slot *slot)
{
  struct i8259 *pic = slot->data;

  // Acknowledge the interrupt
  outb(pic->ports, 0x20);
  if(pic->master)
    outb(pic->master->ports, 0x20);

  // TODO: detect spurious interrupt
  return true;
}

struct slot_ops i8259_slot_ops = {
  .on_enable  = &i8259_slot_on_enable,
  .on_disable = &i8259_slot_on_disable,
  .on_emit    = &i8259_slot_on_emit,
};

static void i8259_reset(struct device *device)
{
  struct i8259 *pic = (struct i8259 *)device; // TODO: container_of
  uint16_t command_port = pic->ports;
  uint16_t data_port    = pic->ports + 1;
  outb(command_port, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(data_port,    pic->base_root);        io_wait();
  outb(data_port,    pic->config);           io_wait();
  outb(data_port,    ICW4_8086);             io_wait();
  outb(data_port,    pic->mask);             io_wait();
}

struct device_ops i8259_device_ops = {
  .reset = &i8259_reset,
};

static int i8259_init(struct i8259 *pic, struct i8259 *master, uint16_t ports, uint8_t base_root, uint8_t base_isa, uint8_t config, uint8_t mask)
{
  pic->device.name = master ? "i8259:slave" : "i8259:master";
  pic->device.ops  = &i8259_device_ops;

  pic->master    = master;
  pic->base_root = base_root;
  pic->base_isa  = base_isa;
  pic->ports     = ports;
  pic->config    = config;
  pic->mask      = mask;
  for(unsigned i=0; i<8; ++i)
  {
    slot_init(&pic->slots[i]);
    pic->slots[i].name = "i8259";
    pic->slots[i].ops  = &i8259_slot_ops;
    pic->slots[i].data = pic;
  }

  if(res_acquire(RES_IRQ_VECTOR, pic->base_root, 8) != 0) return -1;
  if(res_acquire(RES_IOPORT,     pic->ports,     2) != 0) return -1;

  for(unsigned i=0; i<8; ++i)
  {
    slot_connect(irq_slot(IRQ_BUS_ROOT, pic->base_root + i), &pic->slots[i]);
    slot_connect(&pic->slots[i], irq_slot(IRQ_BUS_ISA, pic->base_isa + i));
  }

  return 0;
}

static void i8259_fini(struct i8259 *pic)
{
  res_release(RES_IRQ_VECTOR, pic->base_root, 8);
  res_release(RES_IOPORT,     pic->ports,     2);

  for(unsigned i=0; i<8; ++i)
  {
    slot_disconnect(irq_slot(IRQ_BUS_ROOT, pic->base_root + i), &pic->slots[i]);
    slot_disconnect(&pic->slots[i], irq_slot(IRQ_BUS_ISA, pic->base_isa + i));
  }
}

struct i8259 i8259_master;
struct i8259 i8259_slave;

int i8259_module_init()
{
  i8259_init(&i8259_master, NULL,          PIC_MASTER, 0x20, 0x0, 1 << 2, 0xFB);
  i8259_init(&i8259_slave,  &i8259_master, PIC_SLAVE,  0x28, 0x8, 2,      0xFF);
  return 0;
}

int i8259_module_fini()
{
  // TODO: Check device reference count
  i8259_fini(&i8259_master);
  i8259_fini(&i8259_slave);
  return 0;
}

void i8259_module_up()
{
  device_add(&i8259_master.device);
  device_add(&i8259_slave.device);
}

void i8259_module_down()
{
  device_del(&i8259_master.device);
  device_del(&i8259_slave.device);
}

