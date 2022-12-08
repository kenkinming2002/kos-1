CORE_OBJS = libcore/src/assert.o \
            libcore/src/bitmap.o \
            libcore/src/debug.o \
            libcore/src/slot.o \
            libcore/src/string.o \
            libcore/src/ll.o

libcore.a: CFLAGS+=-Ilibcore/include
libcore.a: $(CORE_OBJS)

ALL_OBJS += $(CORE_OBJS)
ALL_ARS  += libcore.a
