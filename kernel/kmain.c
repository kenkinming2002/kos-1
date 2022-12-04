#include "core/assert.h"
#include "core/debug.h"

#include "debug.h"
#include "hal.h"
#include "mm.h"

#include "hal/irqs.h"

#include "timer/pit.h"

#include "pic/isa.h"
#include "pic/pic8259.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

DEFINE_MODULE(dummy);

void handle_device_not_available()
{
  debug_printf("device not available\n");
}

void handle_timer()
{
  debug_printf("timer interrupt\n");
}

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init(service);
  hal_init();

  pic8259s_init();
  pit_init();
  pit_register_callback(THIS_MODULE, &handle_timer);

  debug_printf("success\n");

  asm volatile ("int $0x7");
  asm volatile ("int $0x80");
  asm volatile ("sti");

  for(;;) asm volatile("hlt");
}
