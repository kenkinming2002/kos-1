#include "exceptions.h"

#include "irqs.h"
#include "module.h"

#include <core/assert.h>

#include <stdbool.h>
#include <stddef.h>

// 0x0 - 0xe
__attribute__((weak)) void handle_devide_by_zero()                 {}
__attribute__((weak)) void handle_debug()                          {}
__attribute__((weak)) void handle_nmi()                            {}
__attribute__((weak)) void handle_breakpoint()                     {}
__attribute__((weak)) void handle_overflow()                       {}
__attribute__((weak)) void handle_bound_range_exceeded()           {}
__attribute__((weak)) void handle_invalid_opcode()                 {}
__attribute__((weak)) void handle_device_not_available()           {}
__attribute__((weak)) void handle_double_fault()                   {}
__attribute__((weak)) void handle_coprocessor_segment_overrun()    {}
__attribute__((weak)) void handle_invalid_tss()                    {}
__attribute__((weak)) void handle_segment_not_present()            {}
__attribute__((weak)) void handle_stack_segment_fault()            {}
__attribute__((weak)) void handle_general_protection_fault()       {}
__attribute__((weak)) void handle_page_fault()                     {}

// 0x10 - 0x15
__attribute__((weak)) void handle_x87_floating_point_exception()   {}
__attribute__((weak)) void handle_alignment_check()                {}
__attribute__((weak)) void handle_machine_check()                  {}
__attribute__((weak)) void handle_simd_floating_point_exception()  {}
__attribute__((weak)) void handle_virtualization_exception()       {}
__attribute__((weak)) void handle_control_protection_exception()   {}

// 0x1C - 0x1E
__attribute__((weak)) void handle_hypervisor_injection_exception() {}
__attribute__((weak)) void handle_vmm_communication_exception()    {}
__attribute__((weak)) void handle_security_exception()             {}

void handle_exceptions(unsigned irq, void *)
{
  switch(irq)
  {
  case 0x0:  handle_devide_by_zero();                 break;
  case 0x1:  handle_debug();                          break;
  case 0x2:  handle_nmi();                            break;
  case 0x3:  handle_breakpoint();                     break;
  case 0x4:  handle_overflow();                       break;
  case 0x5:  handle_bound_range_exceeded();           break;
  case 0x6:  handle_invalid_opcode();                 break;
  case 0x7:  handle_device_not_available();           break;
  case 0x8:  handle_double_fault();                   break;
  case 0x9:  handle_coprocessor_segment_overrun();    break;
  case 0xa:  handle_invalid_tss();                    break;
  case 0xb:  handle_segment_not_present();            break;
  case 0xc:  handle_stack_segment_fault();            break;
  case 0xd:  handle_general_protection_fault();       break;
  case 0xe:  handle_page_fault();                     break;
                                                      break;
  case 0x10: handle_x87_floating_point_exception();   break;
  case 0x11: handle_alignment_check();                break;
  case 0x12: handle_machine_check();                  break;
  case 0x13: handle_simd_floating_point_exception();  break;
  case 0x14: handle_virtualization_exception();       break;
  case 0x15: handle_control_protection_exception();   break;
                                                      break;
  case 0x1C: handle_hypervisor_injection_exception(); break;
  case 0x1D: handle_vmm_communication_exception();    break;
  case 0x1E: handle_security_exception();             break;
  default:
    KASSERT_UNREACHABLE;
  }
}

void exceptions_init()
{
  // No need to keep check of the domain as we do no intend to free it
  struct irq_domain *domain = irq_alloc_domain(0, 0x20);
  domain->handler = &handle_exceptions;
}

