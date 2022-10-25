PAGE_OBJS =

libpage.a: CFLAGS+=-Ilibpage/include

libpage.a: $(PAGE_OBJS)
	$(AR) rcs $@ $(PAGE_OBJS)

