CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld

ASMS = src/boot/loader.s
SRCS = src/boot/main.c
OBJS = $(ASMS:.s=.o) $(SRCS:.c=.o)

all: os.iso
.PHONY: clean

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $^

kernel: src/link.ld $(OBJS)
	$(LD) $(LDFLAGS) -T src/link.ld -o $@ $(OBJS)

os.iso: kernel src/grub.cfg
	./bin/mkiso

clean:
	- rm -f $(OBJS) kernel
