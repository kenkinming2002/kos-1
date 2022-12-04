#include "pic8259.h"

#include "hal/irqs.h"

#include <core/assert.h>
#include <core/debug.h>

#include <stdbool.h>
#include <stddef.h>

DEFINE_MODULE(pic8259);

struct pic8259_line
{
  struct module *module;
  handler_t handler;
};

struct pic8259
{
  uint16_t ports;
  struct pic8259_line lines[16];
};

#define PIC_IOPORTS_MASTER 0x0020
#define PIC_IOPORTS_SLAVE  0x00A0

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

static void pic8259_handle_irq(unsigned irq, void *data)
{
  struct pic8259 *pic = data;
  if(pic->lines[irq].handler)
    pic->lines[irq].handler();
}

/* @params config
 *  master pic: bitmask indicating which interrupt line is cascade
 *  slave  pic: slave id, i.e. the interrupt line of the master pic for which the slave INT line is connected to */
static void pic8259_init(struct pic8259* pic, uint16_t ports, uint8_t config, uint8_t mask)
{
  // TODO: Check for failure
  acquire_ports(THIS_MODULE, ports, 2);
  pic->ports = ports;

  uint16_t command_port = pic->ports;
  uint16_t data_port    = pic->ports + 1;

  // FIXME: This need to be 8-byte aligned
  struct irq_domain *domain = irq_alloc_domain(IRQ_DOMAIN_DYNAMIC_BASE, 8);
  domain->handler = &pic8259_handle_irq;
  domain->data    = pic;

  // Initialize
  outb(command_port, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(data_port,    domain->base);          io_wait();
  outb(data_port,    config);                io_wait();
  outb(data_port,    ICW4_8086);             io_wait();
  outb(data_port,    mask);                  io_wait();
}


static int pic8259_irq_register(struct pic8259* pic, struct module *module, unsigned irq, handler_t handler)
{
  if(pic->lines[irq].module  != NULL ||
     pic->lines[irq].handler != NULL)
    return -1;

  pic->lines[irq].module  = module;
  pic->lines[irq].handler = handler;
  return 0;
}

static int pic8259_irq_deregister(struct pic8259* pic, struct module *module, unsigned irq, handler_t handler)
{
  if(pic->lines[irq].module  != module ||
     pic->lines[irq].handler != handler)
    return -1;

  pic->lines[irq].module  = NULL;
  pic->lines[irq].handler = NULL;
  return 0;
}

static void pic8259_irq_acknowledge(struct pic8259 *pic)
{
  uint16_t command_port = pic->ports;
  outb(command_port, 0x20);
}

static void pic8259_irq_mask(struct pic8259 *pic, unsigned irq)
{
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask |= 1 << irq;
  outb(data_port, mask);

}

static void pic8259_irq_unmask(struct pic8259 *pic, unsigned irq)
{
  uint16_t data_port = pic->ports + 1;
  uint8_t mask = inb(data_port);
  mask &= ~(1 << irq);
  outb(data_port, mask);
}

struct pic8259 master_pic;
struct pic8259 slave_pic;

void pic8259s_init()
{
  pic8259_init(&master_pic, PIC_IOPORTS_MASTER, 0x4, 0xFB);
  pic8259_init(&slave_pic,  PIC_IOPORTS_SLAVE,  2,   0xFF);
}

int pic8259s_irq_register(struct module *module, unsigned irq, handler_t handler)
{
  struct pic8259 *pic;
  if(irq<8)
    pic = &master_pic;
  else if(irq<16)
    pic = &slave_pic;
  else
    KASSERT_UNREACHABLE;
  irq %= 8;

  return pic8259_irq_register(pic, module, irq, handler);
}

int pic8259s_irq_deregister(struct module *module, unsigned irq, handler_t handler)
{
  struct pic8259 *pic;
  if(irq<8)
    pic = &master_pic;
  else if(irq<16)
    pic = &slave_pic;
  else
    KASSERT_UNREACHABLE;
  irq %= 8;

  return pic8259_irq_deregister(pic, module, irq, handler);
}

void pic8259s_irq_acknowledge(unsigned irq)
{
  pic8259_irq_acknowledge(&master_pic);
  if(8 <= irq && irq < 16)
    pic8259_irq_acknowledge(&slave_pic);
}

void pic8259s_irq_mask(unsigned irq)
{
  struct pic8259 *pic;
  if(irq<8)
    pic = &master_pic;
  else if(irq<16)
    pic = &slave_pic;
  else
    KASSERT_UNREACHABLE;
  irq %= 8;

  pic8259_irq_mask(pic, irq);
}

void pic8259s_irq_unmask(unsigned irq)
{
  struct pic8259 *pic;
  if(irq<8)
    pic = &master_pic;
  else if(irq<16)
    pic = &slave_pic;
  else
    KASSERT_UNREACHABLE;
  irq %= 8;

  pic8259_irq_unmask(pic, irq);
}

