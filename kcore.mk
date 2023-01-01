CORE_OBJS = kcore/src/assert.o \
            kcore/src/bitmap.o \
            kcore/src/debug.o \
            kcore/src/string.o \
            kcore/src/ll.o

libkcore.a: CFLAGS+=-Ikcore/include
libkcore.a: $(CORE_OBJS)

ALL_OBJS += $(CORE_OBJS)
ALL_ARS  += libkcore.a
