CORE_OBJS = libcore/src/assert.c.o \
            libcore/src/bitmap.c.o \
            libcore/src/debug.c.o \
            libcore/src/string.c.o \
            libcore/src/ll.c.o

libcore.a: CFLAGS+=-Ilibcore/include
libcore.a: $(CORE_OBJS)

ALL_OBJS += $(CORE_OBJS)
ALL_ARS  += libcore.a
