#include "pic8259.h"

#include <core/assert.h>

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
  unsigned base;
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

/* @params config
 *  master pic: bitmask indicating which interrupt line is cascade
 *  slave  pic: slave id, i.e. the interrupt line of the master pic for which the slave INT line is connected to */
static void pic8259_init(struct pic8259* pic, uint16_t ports, uint8_t config, uint8_t mask)
{
  // TODO: Check for failure
  acquire_ports(THIS_MODULE, ports, 2);

  uint16_t command_port = ports;
  uint16_t data_port    = ports + 1;

  // FIXME: This need to be 8-byte aligned
  pic->base = idt_alloc_range(THIS_MODULE, 8);
  KASSERT(pic->base != IDT_INVALID_VECTOR);

  // Initialize
  outb(command_port, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(data_port,    pic->base);             io_wait();
  outb(data_port,    config);                io_wait();
  outb(data_port,    ICW4_8086);             io_wait();
  outb(data_port,    mask);                  io_wait();
}

static int pic8259_irq_register(struct pic8259* pic, struct module *module, unsigned irq, handler_t handler)
{
  if(pic->lines[irq].module  != NULL ||
     pic->lines[irq].handler != NULL)
    return -1;

  if(idt_register(THIS_MODULE, pic->base + irq, handler) != 0)
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

  if(idt_deregister(THIS_MODULE, pic->base + irq, handler) != 0)
    return -1;

  pic->lines[irq].module  = NULL;
  pic->lines[irq].handler = NULL;
  return 0;
}

struct pic8259 master_pic;
struct pic8259 slave_pic;

void pic8259s_init()
{
  pic8259_init(&master_pic, PIC_IOPORTS_MASTER, 0x4, 0xFB);
  pic8259_init(&slave_pic,  PIC_IOPORTS_SLAVE,  2,   0xFF);
}

static struct pic8259 *pic8259s_map_pic(unsigned irq)
{
  if(irq<8)
    return &master_pic;
  else if(irq<16)
    return &slave_pic;
  else
    KASSERT_UNREACHABLE;
}

static unsigned pic8259s_map_irq(unsigned irq)
{
  if(irq<8)
    return irq;
  else if(irq<16)
    return irq - 8;
  else
    KASSERT_UNREACHABLE;
}

int pic8259s_irq_register(struct module *module, unsigned irq, handler_t handler)
{
  return pic8259_irq_register(pic8259s_map_pic(irq), module, pic8259s_map_irq(irq), handler);
}

int pic8259s_irq_deregister(struct module *module, unsigned irq, handler_t handler)
{
  return pic8259_irq_deregister(pic8259s_map_pic(irq), module, pic8259s_map_irq(irq), handler);
}

