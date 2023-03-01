ARCH_OBJS = libarch/src/access.o \
            libarch/src/locks.o \
            libarch/src/once.o

libarch.a: CFLAGS+=-Ilibarch/include
libarch.a: $(ARCH_OBJS)

ALL_OBJS += $(ARCH_OBJS)
ALL_ARS  += libarch.a

