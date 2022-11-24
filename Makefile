CC=x86_64-elf-gcc
LD=x86_64-elf-ld
AR=x86_64-elf-ar
OBJCOPY=x86_64-elf-objcopy

CFLAGS+=-ffreestanding -fno-stack-protector -mgeneral-regs-only -fPIE
CFLAGS+=-Ikernel
LDFLAGS+=-z max-page-size=4096 -fPIE

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ -c $^

include libcore.mk
include boot.mk
include kernel.mk

os.iso: grub.cfg boot.elf kernel.elf
	./bin/mkiso

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(ALL_OBJS)
	- rm -f $(ALL_ARS)
	- rm -f $(ALL_ELFS)
	- rm -f os.iso
