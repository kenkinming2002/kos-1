KERNEL_SRCS += kos/kmain.c
KERNEL_SRCS += kos/memory.c \
       	       kos/pages.c \
       	       kos/liballoc_1_1.c \
       	       kos/liballoc_hooks.c

KERNEL_OBJS = $(KERNEL_SRCS:=.o)
KERNEL_DEPS = $(KERNEL_SRCS:=.d)

iso/boot/kos.elf: CFLAGS  += -Iinclude -Ilibrt/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -lrt
iso/boot/kos.elf: kos/link.ld librt.a libarch.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_DEPS += $(KERNEL_DEPS)
ALL_ELFS += iso/boot/kos.elf

