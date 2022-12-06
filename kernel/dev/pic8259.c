#include "pic8259.h"

#include "hal/irqs.h"
#include "mm.h"

#include <core/assert.h>

#include <stdbool.h>
#include <stddef.h>

DEFINE_MODULE(pic8259);

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

struct pic8259
{
  struct irqs_source source;

  uint16_t ports;
  uint8_t  config;
  uint8_t  mask;

  struct pic8259 *master;
};

static void pic8259_set_base(struct irqs_source *source, unsigned base)
{
  struct pic8259 *pic = (struct pic8259 *)source;
  uint16_t command_port = pic->ports;
  uint16_t data_port    = pic->ports + 1;
  outb(command_port, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(data_port,    base);                  io_wait();
  outb(data_port,    pic->config);           io_wait();
  outb(data_port,    ICW4_8086);             io_wait();
  outb(data_port,    pic->mask);             io_wait();
}

static void pic8259_acknowledge(struct irqs_source *source)
{
  struct pic8259 *pic = (struct pic8259 *)source;
  uint16_t command_port = pic->ports;
  outb(command_port, 0x20);
  if(pic->master)
    pic8259_acknowledge(&pic->master->source);
}

static void pic8259_mask(struct irqs_source *source, unsigned irq)
{
  struct pic8259 *pic = (struct pic8259 *)source;
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask |= 1 << irq;
  outb(data_port, mask);

}

static void pic8259_unmask(struct irqs_source *source, unsigned irq)
{
  struct pic8259 *pic = (struct pic8259 *)source;
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask &= ~(1 << irq);
  outb(data_port, mask);
}

static struct pic8259 *pic8259_create(uint16_t ports, uint8_t config, uint8_t mask)
{
  if(acquire_ports(THIS_MODULE, ports, 2) != 0)
    return NULL;

  struct pic8259 *pic = kmalloc(sizeof *pic);
  pic->source.set_base    = &pic8259_set_base;
  pic->source.acknowledge = &pic8259_acknowledge;
  pic->source.unmask      = &pic8259_unmask;
  pic->source.mask        = &pic8259_mask;
  pic->ports  = ports;
  pic->config = config;
  pic->mask   = mask;
  pic->master = NULL;
  return pic;
}

static void pic8259_destroy(struct pic8259 *pic)
{
  KASSERT(release_ports(THIS_MODULE, pic->ports, 2) == 0);
  kfree(pic);
}

struct irqs_source *pic8259_master;
struct irqs_source *pic8259_slave;

void pic8259_init()
{
  struct pic8259 *master = pic8259_create(PIC_MASTER, 1 << 2, 0xFB);
  struct pic8259 *slave  = pic8259_create(PIC_SLAVE,  2,      0xFF);
  slave->master = master;

  pic8259_master = &master->source;
  pic8259_slave  = &slave->source;

  acquire_irqs(THIS_MODULE, 0x20, 0x8, pic8259_master);
  acquire_irqs(THIS_MODULE, 0x28, 0x8, pic8259_slave);
}

