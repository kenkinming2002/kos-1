CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

OBJS = src/types/paging.boot.o \
       src/core/assert.boot.o \
       src/boot/debug.boot.o \
       src/boot/loader.boot.o \
       src/boot/main.boot.o

CFLAGS+=-Isrc/

all: os.iso
.PHONY: clean

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.s
	$(CC) $(ASFLAGS) -o $@ -c $^

%.boot.o : %.o
	$(OBJCOPY) --prefix-alloc-sections=.boot --prefix-symbols=boot_ $< $@

kernel: src/link.ld $(OBJS)
	$(LD) $(LDFLAGS) -T src/link.ld -o $@ $(OBJS)

os.iso: kernel src/grub.cfg
	./bin/mkiso

clean:
	- rm -f $(OBJS) kernel
