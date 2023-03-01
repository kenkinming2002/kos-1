ARCH_OBJS = libarch/src/access.c.o \
            libarch/src/locks.S.o \
            libarch/src/once.c.o

libarch.a: CFLAGS+=-Ilibarch/include
libarch.a: $(ARCH_OBJS)

ALL_OBJS += $(ARCH_OBJS)
ALL_ARS  += libarch.a

