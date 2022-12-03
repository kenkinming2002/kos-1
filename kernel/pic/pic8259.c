#include "pic8259.h"

#include <core/assert.h>

#include <stdbool.h>
#include <stddef.h>

static struct module pic8259_module = {
  .name = "pic8259",
};

static unsigned base_master;
static unsigned base_slave;

struct isa_irq_line
{
  struct module *module;
  handler_t handler;
};
static struct isa_irq_line isa_irq_lines[16];

#define PIC_IOPORTS_BEGIN_MASTER 0x0020
#define PIC_IOPORTS_BEGIN_SLAVE  0x00A0
#define PIC_IOPORTS_COUNT        2

#define PIC_COMMAND_MASTER (PIC_IOPORTS_BEGIN_MASTER+0)
#define PIC_COMMAND_SLAVE  (PIC_IOPORTS_BEGIN_SLAVE+0)

#define PIC_DATA_MASTER (PIC_IOPORTS_BEGIN_MASTER+1)
#define PIC_DATA_SLAVE  (PIC_IOPORTS_BEGIN_SLAVE+1)

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

void pic8259_init()
{
  // TODO: Check for failure
  acquire_ports(&pic8259_module, PIC_IOPORTS_BEGIN_MASTER, PIC_IOPORTS_COUNT);
  acquire_ports(&pic8259_module, PIC_IOPORTS_BEGIN_SLAVE,  PIC_IOPORTS_COUNT);

  base_master = idt_alloc_range(&pic8259_module, 8);
  base_slave  = idt_alloc_range(&pic8259_module, 8);

  KASSERT(base_master != IDT_INVALID_VECTOR);
  KASSERT(base_slave  != IDT_INVALID_VECTOR);

  // Initialize
  outb(PIC_COMMAND_MASTER, ICW1_INIT | ICW1_ICW4); io_wait();
  outb(PIC_COMMAND_SLAVE,  ICW1_INIT | ICW1_ICW4); io_wait();

  outb(PIC_DATA_MASTER, base_master); io_wait();
  outb(PIC_DATA_SLAVE,  base_slave);  io_wait();

  outb(PIC_DATA_MASTER, 4); io_wait();
  outb(PIC_DATA_SLAVE,  2); io_wait();

  outb(PIC_DATA_MASTER, ICW4_8086); io_wait();
  outb(PIC_DATA_SLAVE,  ICW4_8086); io_wait();

  // Mask all interrupt except cascade for master
  outb(PIC_DATA_MASTER, 0xFB);
  outb(PIC_DATA_SLAVE,  0xFF);
}

static unsigned isa_irq_to_vector(unsigned irq)
{
  if(irq<8)
    return base_master + irq;
  else if(irq<16)
    return base_slave + irq;
  else
    KASSERT_UNREACHABLE;
}

int isa_irq_register(struct module *module, unsigned irq, handler_t handler)
{
  if(isa_irq_lines[irq].module != NULL ||
     isa_irq_lines[irq].handler != NULL)
    return -1;

  if(idt_register(&pic8259_module, isa_irq_to_vector(irq), handler) != 0)
    return -1;

  isa_irq_lines[irq].module  = module;
  isa_irq_lines[irq].handler = handler;
  return 0;
}

int isa_irq_deregister(struct module *module, unsigned irq, handler_t handler)
{
  if(isa_irq_lines[irq].module  != module ||
     isa_irq_lines[irq].handler != handler)
    return -1;

  if(idt_deregister(&pic8259_module, isa_irq_to_vector(irq), handler) != 0)
    return -1;

  isa_irq_lines[irq].module  = NULL;
  isa_irq_lines[irq].handler = NULL;
  return 0;
}

