ARCH_SRCS = libarch/src/access.c \
            libarch/src/locks.S \
            libarch/src/once.c

ARCH_OBJS = $(ARCH_SRCS:=.o)
ARCH_DEPS = $(ARCH_SRCS:=.d)

libarch.a: CFLAGS+=-Ilibarch/include
libarch.a: $(ARCH_OBJS)

ALL_OBJS += $(ARCH_OBJS)
ALL_DEPS += $(ARCH_DEPS)
ALL_ARS  += libarch.a

