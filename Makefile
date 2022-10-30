CC=x86_64-elf-gcc
LD=x86_64-elf-ld
AR=x86_64-elf-ar
OBJCOPY=x86_64-elf-objcopy

CFLAGS+=-fno-stack-protector
CFLAGS+=-Ikernel
LDFLAGS+=-z max-page-size=4096

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ -c $^

include libcore.mk
include libpage.mk
include libx86.mk
include boot.mk
include kernel.mk

os.iso: grub.cfg boot.elf kernel.elf
	./bin/mkiso

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(OBJS) boot.elf kernel.elf os.iso
