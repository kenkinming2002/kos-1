PAGE_OBJS = libpage/src/region.o

libpage.a: CFLAGS+=-Ilibpage/include

libpage.a: $(PAGE_OBJS)
	$(AR) rcs $@ $(PAGE_OBJS)

