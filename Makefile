CC=x86_64-elf-gcc
LD=x86_64-elf-ld
AR=x86_64-elf-ar
OBJCOPY=x86_64-elf-objcopy

CFLAGS  += -MMD
CFLAGS  += -ffreestanding -fno-stack-protector -mgeneral-regs-only -mno-red-zone -fPIE
LDFLAGS += -nostdlib -z max-page-size=4096
LIBS    += -L. -lgcc

all: os.iso

# Generic rules
%.c.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.S.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

%.a:
	$(AR) rcs $@ $^

%.elf:
	$(CC) $(LDFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

%.tar:
	tar cvf $@ $^

%.iso:
	grub-mkrescue -o $@ $^

include libarch.mk
include libcore.mk
include kboot.mk
include kos.mk

iso/boot/initrd.tar: initrd/empty
os.iso: iso/boot/grub/grub.cfg iso/boot/kboot.elf iso/boot/kos.elf iso/boot/initrd.tar
	grub-mkrescue -o $@ iso

-include $(ALL_DEPS)

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(ALL_OBJS)
	- rm -f $(ALL_DEPS)
	- rm -f $(ALL_ARS)
	- rm -f $(ALL_ELFS)
	- rm -f iso/boot/initrd.tar os.iso
