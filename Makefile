CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

OBJS = src/core/assert.o \
       src/core/print.o \
       src/core/string.o \
       src/boot/debug.o \
       src/boot/loader.o \
       src/boot/kmain.o \
       src/mm/mapping.o

CFLAGS+=-fno-stack-protector
CFLAGS+=-Isrc/

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.s
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
