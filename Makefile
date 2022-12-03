CC=x86_64-elf-gcc
LD=x86_64-elf-ld
AR=x86_64-elf-ar
OBJCOPY=x86_64-elf-objcopy

CFLAGS  += -ffreestanding -fno-stack-protector -mgeneral-regs-only -mno-red-zone -fPIE
LDFLAGS += -nostdlib -z max-page-size=4096
LIBS    += -L. -lgcc

all: os.iso

# Generic rules
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $^

%.a:
	$(AR) rcs $@ $^

%.elf:
	$(CC) $(LDFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

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
