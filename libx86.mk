X86_OBJS =

libx86.a: CFLAGS+=-Ilibx86/include

libx86.a: $(X86_OBJS)
	$(AR) rcs $@ $(X86_OBJS)


