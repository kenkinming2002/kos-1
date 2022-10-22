#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_CMDLINE_LENGTH 256
#define MAX_MMAP_ENTRIES   32

struct boot_params
{
  char cmdline[MAX_CMDLINE_LENGTH];
};
extern struct boot_params boot_params;

void boot_params_init(uint32_t magic, uint32_t addr);

#endif // BOOT_PARAMS_H
