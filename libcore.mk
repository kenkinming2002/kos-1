CORE_SRCS = libcore/src/assert.c \
            libcore/src/bitmap.c \
            libcore/src/debug.c \
            libcore/src/format.c \
            libcore/src/log.c \
            libcore/src/slot.c \
            libcore/src/string.c \
            libcore/src/ll.c

CORE_OBJS = $(CORE_SRCS:=.o)
CORE_DEPS = $(CORE_SRCS:=.d)

libcore.a: CFLAGS+=-Ilibcore/include
libcore.a: $(CORE_OBJS)

ALL_OBJS += $(CORE_OBJS)
ALL_DEPS += $(CORE_DEPS)
ALL_ARS  += libcore.a
