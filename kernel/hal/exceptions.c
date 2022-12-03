#include "exceptions.h"

#include "idt.h"

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

static struct module module = {
  .name = "exceptions",
};

void exceptions_init()
{
  idt_acquire_range(&module, 0, 0x20);

  idt_register(&module, 0x0,  &handle_devide_by_zero);
  idt_register(&module, 0x1,  &handle_debug);
  idt_register(&module, 0x2,  &handle_nmi);
  idt_register(&module, 0x3,  &handle_breakpoint);
  idt_register(&module, 0x4,  &handle_overflow);
  idt_register(&module, 0x5,  &handle_bound_range_exceeded);
  idt_register(&module, 0x6,  &handle_invalid_opcode);
  idt_register(&module, 0x7,  &handle_device_not_available);
  idt_register(&module, 0x8,  &handle_double_fault);
  idt_register(&module, 0x9,  &handle_coprocessor_segment_overrun);
  idt_register(&module, 0xa,  &handle_invalid_tss);
  idt_register(&module, 0xb,  &handle_segment_not_present);
  idt_register(&module, 0xc,  &handle_stack_segment_fault);
  idt_register(&module, 0xd,  &handle_general_protection_fault);
  idt_register(&module, 0xe,  &handle_page_fault);

  idt_register(&module, 0x10, &handle_x87_floating_point_exception);
  idt_register(&module, 0x11, &handle_alignment_check);
  idt_register(&module, 0x12, &handle_machine_check);
  idt_register(&module, 0x13, &handle_simd_floating_point_exception);
  idt_register(&module, 0x14, &handle_virtualization_exception);
  idt_register(&module, 0x15, &handle_control_protection_exception);

  idt_register(&module, 0x1C, &handle_hypervisor_injection_exception);
  idt_register(&module, 0x1D, &handle_vmm_communication_exception);
  idt_register(&module, 0x1E, &handle_security_exception);
}

