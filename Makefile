CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

OBJS = src/core/assert.o \
       src/core/bitmap.o \
       src/core/print.o \
       src/core/string.o \
       src/boot_params.o \
       src/debug.o \
       src/kmain.o \
       src/loader.o \
       src/mm/pages.o

CFLAGS+=-fno-stack-protector
CFLAGS+=-Isrc/

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ -c $^

# Kernel and iso
kernel: src/link.ld $(OBJS)
	$(CC) $(LDFLAGS) -T src/link.ld -o $@ $(OBJS) -nostdlib -lgcc

os.iso: kernel src/grub.cfg
	./bin/mkiso

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(OBJS) kernel os.iso
