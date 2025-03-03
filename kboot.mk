BOOT_SRCS = kboot/src/entry.S \
	    kboot/src/multiboot2.S \
	    kboot/src/bmain.c \
	    kboot/src/kboot/all.c \
	    kboot/src/kboot/alloc.c \
	    kboot/src/kboot/info.c \
	    kboot/src/kboot/mmap.c \
	    kboot/src/file.c \
	    kboot/src/kernel.c \
	    kboot/src/initrd.c

BOOT_OBJS = $(BOOT_SRCS:=.o)
BOOT_DEPS = $(BOOT_SRCS:=.d)

iso/boot/kboot.elf: CFLAGS  += -Iinclude -Ilibrt/include -Ikboot/include -Ikboot/src
iso/boot/kboot.elf: LDFLAGS += -Tkboot/link.ld
iso/boot/kboot.elf: LIBS    += -lrt
iso/boot/kboot.elf: kboot/link.ld librt.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_DEPS += $(BOOT_DEPS)
ALL_ELFS += iso/boot/kboot.elf
