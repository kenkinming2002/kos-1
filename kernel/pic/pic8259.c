#include "pic8259.h"

#include "hal.h"

#include <core/assert.h>

static struct module module = {
  .name = "pic8259",
};

#define PIC_IOPORTS_BEGIN_MASTER 0x0020
#define PIC_IOPORTS_BEGIN_SLAVE  0x00A0
#define PIC_IOPORTS_COUNT        2

#define PIC_COMMAND_MASTER (PIC_IOPORTS_BEGIN_MASTER+0)
#define PIC_COMMAND_SLAVE  (PIC_IOPORTS_BEGIN_SLAVE+0)

#define PIC_DATA_MASTER (PIC_IOPORTS_BEGIN_MASTER+1)
#define PIC_DATA_SLAVE  (PIC_IOPORTS_BEGIN_SLAVE+1)

static unsigned base_master;
static unsigned base_slave;

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
  acquire_ports(&module, PIC_IOPORTS_BEGIN_MASTER, PIC_IOPORTS_COUNT);
  acquire_ports(&module, PIC_IOPORTS_BEGIN_SLAVE,  PIC_IOPORTS_COUNT);

  base_master = idt_alloc_range(&module, 8);
  base_slave  = idt_alloc_range(&module, 8);

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
