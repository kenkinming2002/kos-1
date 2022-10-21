CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

CFLAGS+=-fno-stack-protector
CFLAGS+=-Iboot -Ikernel

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ -c $^

include boot.mk
include kernel.mk

os.iso: grub.cfg boot.elf kernel.elf
	./bin/mkiso

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(OBJS) boot.elf kernel.elf os.iso
