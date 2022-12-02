#include "core/assert.h"
#include "core/debug.h"

#include "debug.h"
#include "mm/pages.h"
#include "hal/ports.h"
#include "hal/gdt.h"
#include "hal/idt.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

struct module dummy = {
  .name = "dummy"
};

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init_pages(service);

  KASSERT(acquire_ports(&dummy, 0, 16)  ==  0);
  KASSERT(acquire_ports(&dummy, 2, 4)   == -1);
  KASSERT(acquire_ports(&dummy, 16, 24) ==  0);
  KASSERT(release_ports(&dummy, 2, 4)   == -1);
  KASSERT(acquire_ports(&dummy, 3, 5)   == -1);
  KASSERT(release_ports(&dummy, 0, 16)  ==  0);
  KASSERT(acquire_ports(&dummy, 3, 5)   ==  0);

  debug_printf("success\n");

  gdt_init();
  idt_init();

  asm volatile ("int $0x80");
  for(;;) asm volatile("hlt");
}
