CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

OBJS = src/boot/loader.boot.o \
       src/boot/main.boot.o

all: os.iso
.PHONY: clean

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $^

%.boot.o : %.o
	$(OBJCOPY) --prefix-sections=.boot --prefix-symbols=boot_ $< $@

kernel: src/link.ld $(OBJS)
	$(LD) $(LDFLAGS) -T src/link.ld -o $@ $(OBJS)

os.iso: kernel src/grub.cfg
	./bin/mkiso

clean:
	- rm -f $(OBJS) kernel
