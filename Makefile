CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

OBJS = src/types/paging.boot.o \
       src/core/assert.boot.o \
       src/core/print.boot.o \
       src/core/string.boot.o \
       src/boot/alloc.boot.o \
       src/boot/debug.boot.o \
       src/boot/loader.boot.o \
       src/boot/main.boot.o

CFLAGS+=-fno-stack-protector
CFLAGS+=-Isrc/

all: os.iso

# Generic rules
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.s
	$(CC) $(ASFLAGS) -o $@ -c $^

%.boot.o : %.o
	$(OBJCOPY) --prefix-alloc-sections=.boot --prefix-symbols=boot_ $< $@

%.boot.a : %.a
	$(OBJCOPY) --prefix-alloc-sections=.boot --prefix-symbols=boot_ $< $@

# Libgcc
SYSTEM_LIBGCC=$(shell i686-elf-gcc $(CFLAGS) -print-libgcc-file-name)
libgcc.a: $(SYSTEM_LIBGCC)
	$(OBJCOPY) --remove-section=.eh_frame $< $@

# Kernel and iso
kernel: src/link.ld $(OBJS) libgcc.boot.a libgcc.a
	$(LD) $(LDFLAGS) -T src/link.ld -o $@ $(OBJS) -L./ -lgcc -lgcc.boot

os.iso: kernel src/grub.cfg
	./bin/mkiso

# PHONY rule
.PHONY: clean
clean:
	- rm -f $(OBJS) libgcc.boot.a libgcc.a kernel os.iso
