CORE_OBJS = core/src/assert.o \
            core/src/bitmap.o \
            core/src/debug.o \
            core/src/string.o

libcore.a: $(CORE_OBJS)
	$(AR) rcs $@ $(CORE_OBJS)

