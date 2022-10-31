X86_OBJS =

libx86.a: CFLAGS+=-Ilibx86/include

libx86.a: $(X86_OBJS)
	$(AR) rcs $@ $(X86_OBJS)

ALL_OBJS += $(X86_OBJS)
ALL_ARS  += libx86.a
