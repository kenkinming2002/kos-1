RT_SRCS = librt/src/core/assert.c \
          librt/src/core/bitmap.c \
          librt/src/core/format.c \
          librt/src/core/log.c \
          librt/src/core/log_serial.c \
          librt/src/core/slot.c \
          librt/src/core/string.c \
          librt/src/core/ll.c \
          librt/src/core/locks.S \
          librt/src/core/once.c \
          librt/src/asm/io.c

RT_OBJS = $(RT_SRCS:=.o)
RT_DEPS = $(RT_SRCS:=.d)

librt.a: CFLAGS+=-Ilibrt/include
librt.a: $(RT_OBJS)

ALL_OBJS += $(RT_OBJS)
ALL_DEPS += $(RT_DEPS)
ALL_ARS  += librt.a
