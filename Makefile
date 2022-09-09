CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld

ASMS = src/loader.s
SRCS =
OBJS = $(ASMS:.s=.o) $(SRCS:.c=.o)

all: os.iso
.PHONY: clean

%.o: %.c
	$(CC) -o $@ $^

%.o: %.s
	$(AS) -o $@ $^

kernel: src/link.ld $(OBJS)
	$(LD) -T src/link.ld -o $@ $(OBJS)

os.iso: kernel src/grub.cfg
	./bin/mkiso

clean:
	- rm -f $(OBJS) kernel
