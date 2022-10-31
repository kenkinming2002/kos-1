CORE_OBJS = libcore/src/assert.o \
            libcore/src/bitmap.o \
            libcore/src/debug.o \
            libcore/src/string.o

libcore.a: CFLAGS+=-Ilibcore/include

libcore.a: $(CORE_OBJS)
	$(AR) rcs $@ $(CORE_OBJS)

ALL_OBJS += $(CORE_OBJS)
ALL_ARS  += libcore.a
