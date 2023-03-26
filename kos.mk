KERNEL_SRCS = kos/kmain.c \
       	      kos/arch/all.c \
       	      kos/arch/segmentation.c \
       	      kos/arch/segmentation.S \
       	      kos/arch/interrupt.c \
       	      kos/arch/interrupt.S \
       	      kos/core/mm/all.c \
       	      kos/core/mm/pages.c \
       	      kos/core/mm/liballoc_1_1.c \
       	      kos/core/mm/liballoc_hooks.c \
       	      kos/core/hal/module.c \
       	      kos/core/hal/device.c \
       	      kos/core/hal/res.c \
       	      kos/core/hal/irq.c \
       	      kos/core/hal/timer.c \
       	      kos/modules/dev/i8253.c \
       	      kos/modules/dev/i8259.c

KERNEL_OBJS = $(KERNEL_SRCS:=.o)
KERNEL_DEPS = $(KERNEL_SRCS:=.d)

iso/boot/kos.elf: CFLAGS  += -Iinclude -Ilibrt/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -lrt
iso/boot/kos.elf: kos/link.ld librt.a libarch.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_DEPS += $(KERNEL_DEPS)
ALL_ELFS += iso/boot/kos.elf

